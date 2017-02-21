#include "Game\Game.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "GameObject\ActorCreator.h"
#include "Input\KeyboardEventDispatcher.h"
#include "Jobs\CreateActorDeleteFileDataJob.h"
#include "Jobs\FileLoadJob.h"
#include "Jobs\JobSystem.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"

// game includes
#include "Game\GameData.h"
#include "Game\GameUtils.h"
#include "Game\Player.h"

namespace game {

bool StartUp()
{
	// create an instance of the game
	Game* game = Game::Create();
	if (game == nullptr)
	{
		LOG_ERROR("Could not create an instance of MonsterChase!");
		return false;
	}

	// initialize the game
	bool success = game->Init();
	if (success)
	{
		LOG("-------------------- MonsterChase StartUp --------------------");
	}
	else
	{
		LOG_ERROR("Could not initialize MonsterChase!");
	}

	return success;
}

bool Restart()
{
	game::Shutdown();
	return game::StartUp();
}

void Shutdown()
{
	Game::Destroy();
	LOG("-------------------- MonsterChase Shutdown --------------------");
}

// static member initialization
Game* Game::instance_ = nullptr;

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
	player_(nullptr),
	keyboard_event_(engine::input::KeyboardEvent::Create())
{
	ASSERT(keyboard_event_);
}

Game::~Game()
{
	// delete the player
	SAFE_DELETE(player_);

	// delete the monsters
	actors_.clear();

	// tell the engine we no longer want to be ticked
	engine::time::Updater::Get()->RemoveTickable(this);

	engine::input::KeyboardEventDispatcher::Get()->RemoveListener(keyboard_event_);
}

bool Game::Init()
{
	ASSERT(game_state_ == GameStates::kGameStateBegin);

	// load game data
	if (!LoadGameData())
	{
		return false;
	}

	srand(static_cast<unsigned int>(time(0)));

	// create the player
	CreatePlayer();
	LOG("Created the player...");

	// create the monsters
	engine::gameobject::ActorCreator::CreateActorsFromFile(GameData::MONSTERS_LUA_FILE_NAME, actors_);

	// register for update events
	engine::time::Updater::Get()->AddTickable(this);

	// register for key events
	keyboard_event_->SetOnKeyPressed(std::bind(&Game::OnKeyPressed, this, std::placeholders::_1));
	engine::input::KeyboardEventDispatcher::Get()->AddListener(keyboard_event_);

	game_state_ = GameStates::kGameStateRunning;

	return true;
}

bool Game::LoadGameData()
{
	// load textures
	engine::util::FileUtils::Get()->ReadFile(GameData::PLAYER_TEXTURE_NAME, true);
	engine::util::FileUtils::Get()->ReadFile(GameData::SILLY_MONSTER_TEXTURE_NAME, true);
	engine::util::FileUtils::Get()->ReadFile(GameData::SMART_MONSTER_TEXTURE_NAME, true);
	return true;
}

void Game::Update(float dt)
{
	bool quit = false;
	GLib::Service(quit);
	
	// request the engine to quit if we need to
	if (quit || game_state_ == GameStates::kGameStateQuit)
	{
		engine::RequestShutdown();
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
	switch (i_key_id)
	{
	case 'M':
		CreateActor(engine::data::PooledString(GameData::MONSTER_LUA_FILE_NAME));
		break;
	case 'Q':
		game_state_ = GameStates::kGameStateQuit;
		break;
	/*case 'R':
		game_state_ = GameStates::kGameStateRestart;
		break;*/
	}
}

void Game::CreatePlayer()
{
	// create the player at the center of the grid
	player_ = new Player();
}

void Game::CreateActor(const engine::data::PooledString& i_file_name)
{
	// validate inputs
	ASSERT(i_file_name);

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

	std::lock_guard<std::mutex> lock(new_actors_mutex_);
	new_actors_.push_back(i_actor);
}

} // namespace monsterchase
