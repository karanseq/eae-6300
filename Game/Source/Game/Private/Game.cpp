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
#include "Game\LevelData.h"
#include "Game\Player.h"

namespace game {

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
    const engine::util::FileUtils::FileData config_file_data = engine::util::FileUtils::Get()->ReadFile(GameData::GAME_CONFIG_FILE);
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
    level_number_(1),
    level_data_(nullptr),
    player_(nullptr),
    keyboard_event_(nullptr),
    move_enemies_event_(nullptr),
    fire_enemy_bullet_event_(nullptr)
{
    game_state_ = GameStates::kGameStateLoading;
    game_data_.LoadAssetsListedInConfig(std::bind(&Game::OnAssetLoadingComplete, this), std::bind(&Game::OnAssetLoadingFailed, this));
}

Game::~Game()
{
    Reset();
}

bool Game::Init()
{
    ASSERT(game_state_ == GameStates::kGameStateBegin);

    // make sure we don't init repeatedly
    if (init_successful_)
    {
        return false;
    }
    init_successful_ = true;

    LOG("%s", __FUNCTION__);

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

    // create the level
    CreateLevel();

    // create the player
    CreatePlayer();

    return true;
}

void Game::Reset()
{
    ASSERT(game_state_ == GameStates::kGameStateQuit || game_state_ == GameStates::kGameStateRestart);

    // make sure we don't reset repeatedly
    if (!init_successful_)
    {
        return;
    }
    init_successful_ = false;

    LOG("%s", __FUNCTION__);

    move_enemies_event_ = nullptr;
    fire_enemy_bullet_event_ = nullptr;
    engine::time::Updater::Get()->RemoveAllTimerEvents();

    DestroyPlayer();

    DestroyLevel();

    engine::events::EventDispatcher::Get()->RemoveKeyboardEventListener(keyboard_event_);
    keyboard_event_ = nullptr;

    // tell the engine we no longer want collision events
    engine::physics::Collider::Get()->SetCollisionListener(nullptr);

    // tell the engine we no longer want to be ticked
    engine::time::Updater::Get()->RemoveTickable(this);
}

void Game::OnAssetLoadingComplete()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateBegin;
    Init();
}

void Game::OnAssetLoadingFailed()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateQuit;
    engine::InitiateShutdown();
}

void Game::OnLevelLoadingComplete()
{
    LOG("%s", __FUNCTION__);

    move_enemies_event_ = engine::events::TimerEvent::Create(std::bind(&Game::OnMoveEnemiesTimerElapsed, this), level_data_->level_.enemy_move_interval_, -1);
    engine::time::Updater::Get()->AddTimerEvent(move_enemies_event_);

    fire_enemy_bullet_event_ = engine::events::TimerEvent::Create(std::bind(&Game::OnFireEnemyBulletTimerElapsed, this), level_data_->level_.enemy_fire_rate_, -1);
    engine::time::Updater::Get()->AddTimerEvent(fire_enemy_bullet_event_);

    game_state_ = GameStates::kGameStateRunning;
}

void Game::OnLevelLoadingFailed()
{
    LOG("%s", __FUNCTION__);
    game_state_ = GameStates::kGameStateQuit;
    engine::InitiateShutdown();
}

void Game::OnKeyPressed(unsigned int i_key_id)
{
    if (game_state_ != GameStates::kGameStateRunning &&
        game_state_ != GameStates::kGameStatePaused)
    {
        return;
    }

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
        if (engine::IsPaused())
        {
            engine::Resume();
            game_state_ = GameStates::kGameStateRestart;
        }
    }
    else if (i_key_id == 'Q')
    {
        game_state_ = GameStates::kGameStateQuit;
        engine::InitiateShutdown();
    }
}

void Game::OnMoveEnemiesTimerElapsed()
{
    LOG("%s", __FUNCTION__);
}

void Game::OnFireEnemyBulletTimerElapsed()
{
    LOG("%s", __FUNCTION__);
}

void Game::Tick(float dt)
{
    if (game_state_ == GameStates::kGameStateRestart)
    {
        Reset();
        game_state_ = GameStates::kGameStateBegin;
        Init();
        return;
    }
}

void Game::OnCollision(const engine::physics::CollisionPair& i_collision_pair)
{
    LOG("%s", __FUNCTION__);
}

void Game::CreatePlayer()
{
    player_ = new Player(game_data_.GetPlayerLuaFileName());
}

void Game::DestroyPlayer()
{
    SAFE_DELETE(player_);
}

void Game::CreateLevel()
{
    level_data_ = new LevelData();

    char buf[512];
    sprintf_s(buf, "%s%02d.lua", game_data_.GetLevelLuaFilePath().GetString(), level_number_);

    level_data_->LoadLevel(engine::util::FileUtils::Get()->GetFileFromCache(engine::data::HashedString::Hash(buf)), std::bind(&Game::OnLevelLoadingComplete, this), std::bind(&Game::OnLevelLoadingFailed, this));
}

void Game::DestroyLevel()
{
    SAFE_DELETE(level_data_);
}

} // namespace game
