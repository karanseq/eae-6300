#include "Game\Game.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "Events\EventDispatcher.h"
#include "GameObject\ActorCreator.h"
#include "Jobs\CreateActorFromFileJob.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\Player.h"

namespace game {

static float X(-0.4f * Game::SCREEN_WIDTH);
static float Y(0.4f * Game::SCREEN_HEIGHT);

bool StartUp()
{
    LOG("-------------------- Game StartUp --------------------");

    // create a team of workers for the game
    if (!engine::jobs::JobSystem::Get()->CreateTeam(engine::data::PooledString("GameTeam"), 10))
    {
        LOG_ERROR("Could not create a job team for the game!");
        return false;
    }

    // load the game config
    const engine::util::FileUtils::FileData config_file_data = engine::util::FileUtils::Get()->ReadFile(GameData::GAME_CONFIG_FILE, true);
    ASSERT(config_file_data.file_contents && config_file_data.file_size > 0);

    // create an instance of the game
    Game* game = Game::Create();
    if (game == nullptr)
    {
        LOG_ERROR("Could not create an instance of the game!");
        return false;
    }

    srand(static_cast<unsigned int>(time(0)));

    return true;
}

void Shutdown()
{
    Game::Destroy();
    LOG("-------------------- Game Shutdown --------------------");
}

// static member initialization
Game* Game::instance_ = nullptr;

Game* Game::Create()
{
    // create the singleton instance of Game
    if (!Game::instance_)
    {
        Game::instance_ = new Game();
    }
    return Game::instance_;
}

void Game::Destroy()
{
    // destroy the singleton instance of Game
    SAFE_DELETE(Game::instance_);
}

Game::Game() : game_state_(GameStates::kGameStateNone),
    init_successful_(false),
    player_(nullptr),
    keyboard_event_(nullptr),
    move_enemies_event_(nullptr),
    fire_enemy_bullet_event_(nullptr)
{
    game_state_ = GameStates::kGameStateLoading;
    game_data_.LoadAssetsListedInConfig(std::bind(&Game::OnLoadingComplete, this), std::bind(&Game::OnLoadingFailed, this));
}

Game::~Game()
{
    Reset();
}

void Game::OnLoadingComplete()
{
    game_state_ = GameStates::kGameStateBegin;
    Init();
}

void Game::OnLoadingFailed()
{
    game_state_ = GameStates::kGameStateQuit;
    engine::InitiateShutdown();
}

bool Game::Init()
{
    ASSERT(game_state_ == GameStates::kGameStateBegin || game_state_ == GameStates::kGameStateRestart);

    if (init_successful_)
    {
        return false;
    }
    init_successful_ = true;

    // create the player
    CreatePlayer();

    // register for update events
    engine::time::Updater::Get()->AddTickable(this);

    // register for collision events
    engine::physics::Collider::Get()->SetCollisionListener(this);

    // create a new keyboard event
    keyboard_event_ = engine::events::KeyboardEvent::Create();
    ASSERT(keyboard_event_);

    // register for key events
    keyboard_event_->SetOnKeyPressed(std::bind(&Game::OnKeyPressed, this, std::placeholders::_1));
    engine::events::EventDispatcher::Get()->AddKeyboardEventListener(keyboard_event_);

    game_state_ = GameStates::kGameStateRunning;

    //engine::time::Updater::Get()->AddTimerEvent(engine::events::TimerEvent::Create([&]() {
    //    LOG("DummyTimer!");
    //    CreateActor("Data\\Actors\\Enemy_01.lua");
    //}, 0.01f, 100));

    for (uint8_t i = 0; i < 50; ++i)
    {
        static const char* names[4] = { "Data\\Actors\\Enemy_01.lua", "Data\\Actors\\Enemy_02.lua", "Data\\Actors\\Enemy_03.lua", "Data\\Actors\\Brick_01.lua" };
        CreateActor(names[i % 4]);
    }

    return true;
}

void Game::Reset()
{
    ASSERT(game_state_ == GameStates::kGameStateQuit || game_state_ == GameStates::kGameStateRestart);

    if (!init_successful_)
    {
        return;
    }
    init_successful_ = false;

    DestroyPlayer();
    actors_.clear();
    new_actors_.clear();

    engine::time::Updater::Get()->RemoveAllTimerEvents();

    // tell the engine we no longer want collision events
    engine::physics::Collider::Get()->SetCollisionListener(nullptr);

    // tell the engine we no longer want to be ticked
    engine::time::Updater::Get()->RemoveTickable(this);

    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
    keyboard_event_ = nullptr;

    X = -0.5f * Game::SCREEN_WIDTH;
    Y = 0.45f * Game::SCREEN_HEIGHT;
}

void Game::Tick(float dt)
{
    if (game_state_ == GameStates::kGameStateRestart)
    {
        Reset();
        Init();
        return;
    }

    const size_t num_actors = actors_.size();
    for (size_t i = 0; i < num_actors; ++i)
    {
        engine::math::Vec3D position = actors_[i]->GetGameObject()->GetPosition();
        position.x(position.x() < -0.5f * Game::SCREEN_WIDTH ? 0.5f * Game::SCREEN_WIDTH : (position.x() > 0.5f * Game::SCREEN_WIDTH ? -0.5f * Game::SCREEN_WIDTH : position.x()));
        position.y(position.y() < -0.5f * Game::SCREEN_HEIGHT ? 0.5f * Game::SCREEN_HEIGHT : (position.y() > 0.5f * Game::SCREEN_HEIGHT ? -0.5f * Game::SCREEN_HEIGHT : position.y()));
        actors_[i]->GetGameObject()->SetPosition(position);

        engine::math::Vec3D impulse;
        int64_t j(i + 1);
        impulse.x(j % 2 ? -j * 0.2f : j * 0.2f);
        impulse.y(j % 3 ? j * 0.15f : -j * 0.2f);

        actors_[i]->GetPhysicsObject().Lock()->ApplyImpulse(impulse);
    }

    if (!new_actors_.empty())
    {
        std::lock_guard<std::mutex> lock(new_actors_mutex_);
        actors_.insert(actors_.end(), new_actors_.begin(), new_actors_.end());
        new_actors_.clear();
    }
}

void Game::OnKeyPressed(unsigned int i_key_id)
{
    if (i_key_id == 'P')
    {
        if (engine::IsPaused())
        {
            engine::Resume();
            game_state_ = GameStates::kGameStateRunning;
        }
        else
        {
            engine::Pause();
            game_state_ = GameStates::kGameStatePaused;
        }
    }
    else if (i_key_id == 'R')
    {
        game_state_ = GameStates::kGameStateRestart;
    }
    else if (i_key_id == 'Q')
    {
        game_state_ = GameStates::kGameStateQuit;
        engine::InitiateShutdown();
    }
}

void Game::CreatePlayer()
{
    player_ = new Player();
}

void Game::DestroyPlayer()
{
    SAFE_DELETE(player_);
}

void Game::OnCollision(const engine::physics::CollisionPair& i_collision_pair)
{
    LOG("%s", __FUNCTION__);
}

void Game::CreateActor(const engine::data::PooledString& i_file_name)
{
    // validate inputs
    ASSERT(i_file_name.GetLength() > 0);

    engine::jobs::FileLoadJob* file_load_job = new engine::jobs::FileLoadJob(i_file_name, std::bind(&Game::OnFileLoaded, this, std::placeholders::_1));
    engine::jobs::JobSystem::Get()->AddJob(file_load_job, engine::data::PooledString("GameTeam"));
}

void Game::OnFileLoaded(const engine::util::FileUtils::FileData i_file_data) const
{
    // validate inputs
    ASSERT(i_file_data.file_contents && i_file_data.file_size > 0);

    engine::jobs::CreateActorFromFileJob* actor_creator_job = new engine::jobs::CreateActorFromFileJob(i_file_data, std::bind(&Game::OnActorCreated, this, std::placeholders::_1));
    engine::jobs::JobSystem::Get()->AddJob(actor_creator_job, engine::data::PooledString("GameTeam"));
}

void Game::OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor> i_actor) const
{
    // validate inputs
    ASSERT(i_actor);

    // give this actor a random position
    //i_actor->GetGameObject()->SetPosition(GameUtils::GetRandomVec3D(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0));

    i_actor->GetGameObject()->SetPosition(engine::math::Vec3D(X, Y));
    X += 200.0f;
    Y -= X > 0.4f * Game::SCREEN_WIDTH ? 120.0f : 0.0f;
    X = X > 0.4f * Game::SCREEN_WIDTH ? -0.4f * Game::SCREEN_WIDTH : X;

    std::lock_guard<std::mutex> lock(new_actors_mutex_);
    new_actors_.push_back(i_actor);
}

} // namespace game
