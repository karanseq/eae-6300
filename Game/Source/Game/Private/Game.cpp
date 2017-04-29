#include "Game\Game.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "GameObject\ActorCreator.h"
#include "Events\EventDispatcher.h"
#include "Jobs\CreateActorDeleteFileDataJob.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"

// game includes
#include "Game\Asteroid.h"
#include "Game\GameData.h"
#include "Game\GameUtils.h"
#include "Game\Player.h"

namespace game {

bool StartUp()
{
    LOG("-------------------- MonsterChase StartUp --------------------");
    // create an instance of the game
    Game* game = Game::Create();
    if (game == nullptr)
    {
	    LOG_ERROR("Could not create an instance of MonsterChase!");
	    return false;
    }

    // load game data
    if (!LoadGameData())
    {
        return false;
    }

    srand(static_cast<unsigned int>(time(0)));

    // initialize the game
    bool success = game->Init();
    if (!success)
    {
	    LOG_ERROR("Could not initialize MonsterChase!");
    }

    return success;
}

bool LoadGameData()
{
    // load textures
    engine::util::FileUtils::Get()->ReadFile(GameData::PLAYER_TEXTURE_NAME, true);
    engine::util::FileUtils::Get()->ReadFile(GameData::SILLY_MONSTER_TEXTURE_NAME, true);
    engine::util::FileUtils::Get()->ReadFile(GameData::SMART_MONSTER_TEXTURE_NAME, true);
    return true;
}

void Shutdown()
{
	Game::Destroy();
	LOG("-------------------- MonsterChase Shutdown --------------------");
}

// static member initialization
Game* Game::instance_ = nullptr;
static uint16_t wait_before_quit = 0;

Game* Game::Create()
{
	// create the singleton instance of MonsterChase
	if (!Game::instance_)
	{
		Game::instance_ = new Game();
	}
	return Game::instance_;
}

void Game::Destroy()
{
	// destroy the singleton instance of MonsterChase
	SAFE_DELETE(Game::instance_);
}

Game::Game() : game_state_(GameStates::kGameStateBegin),
	player_01_(nullptr),
	keyboard_event_(engine::events::KeyboardEvent::Create())
{
	ASSERT(keyboard_event_);

    srand(static_cast<unsigned int>(time(0)));
}

Game::~Game()
{
    Reset();
}

bool Game::Init()
{
	ASSERT(game_state_ == GameStates::kGameStateBegin || game_state_ == GameStates::kGameStateRestart);

	// create the player
	CreatePlayer();

	// create the asteroids
    CreateAsteroids();

	// register for update events
	engine::time::Updater::Get()->AddTickable(this);

	// register for key events
	keyboard_event_->SetOnKeyPressed(std::bind(&Game::OnKeyPressed, this, std::placeholders::_1));
    engine::events::EventDispatcher::Get()->AddKeyboardEventListener(keyboard_event_);

	game_state_ = GameStates::kGameStateRunning;

	return true;
}

void Game::Reset()
{
    ASSERT(game_state_ == GameStates::kGameStateQuit || game_state_ == GameStates::kGameStateRestart);

    DestroyPlayer();
    DestroyAsteroids();
    actors_.clear();
    new_actors_.clear();

    // tell the engine we no longer want to be ticked
    engine::time::Updater::Get()->RemoveTickable(this);

    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
}

void Game::Update(float dt)
{
    if (game_state_ == GameStates::kGameStateRestart)
    {
        Reset();
        ASSERT(Init());
        return;
    }

    if (!new_actors_.empty())
    {
        std::lock_guard<std::mutex> lock(new_actors_mutex_);
        actors_.insert(actors_.end(), new_actors_.begin(), new_actors_.end());
        new_actors_.clear();
    }

    ++wait_before_quit;
    if (wait_before_quit >= 1000)
    {
        game_state_ = GameStates::kGameStateQuit;
        engine::InitiateShutdown();
    }
    else if (wait_before_quit % 100 == 0)
    {
        LOG("COUNT:%d", wait_before_quit);
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
    player_01_ = new Player();
    player_01_->SetKeys('A', 'D', 'W', 'S');
}

void Game::DestroyPlayer()
{
    SAFE_DELETE(player_01_);
}

void Game::CreateAsteroids()
{
    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>> actors;
    engine::gameobject::ActorCreator::CreateActorsFromFile(GameData::MONSTERS_LUA_FILE_NAME, actors);

    for (const auto& actor : actors)
    {
        asteroids_.push_back(new Asteroid(actor));
    }
}

void Game::DestroyAsteroids()
{
    for (auto& asteroid : asteroids_)
    {
        delete asteroid;
    }
    asteroids_.clear();
}

void Game::CreateActor(const engine::data::PooledString& i_file_name)
{
	// validate inputs
	ASSERT(i_file_name.GetLength() > 0);

	engine::jobs::FileLoadJob* file_load_job = new engine::jobs::FileLoadJob(i_file_name, std::bind(&Game::OnFileLoaded, this, std::placeholders::_1));
	engine::jobs::JobSystem::Get()->AddJob(file_load_job, engine::data::PooledString("EngineJobs"));
}

void Game::OnFileLoaded(engine::util::FileUtils::FileData i_file_data)
{
	// validate inputs
	ASSERT(i_file_data.file_contents && i_file_data.file_size > 0);

	engine::jobs::CreateActorDeleteFileDataJob* actor_creator_job = new engine::jobs::CreateActorDeleteFileDataJob(i_file_data, std::bind(&Game::OnActorCreated, this, std::placeholders::_1));
	engine::jobs::JobSystem::Get()->AddJob(actor_creator_job, engine::data::PooledString("EngineJobs"));
}

void Game::OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor> i_actor)
{
    // validate inputs
    ASSERT(i_actor);

    // give this actor a random position
    i_actor->GetGameObject()->SetPosition(GameUtils::GetRandomVec3D(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0));

    std::lock_guard<std::mutex> lock(new_actors_mutex_);
    new_actors_.push_back(i_actor);
}

} // namespace monsterchase
