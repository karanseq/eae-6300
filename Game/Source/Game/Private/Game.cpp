#include "Game\Game.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "GameObject\ActorCreator.h"
#include "Input\KeyboardEventDispatcher.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameData.h"
#include "Game\GameUtils.h"
#include "Game\Player.h"

namespace game {

bool StartUp()
{
	// create an instance of the game
	Game* mc_instance = Game::Create();
	if (mc_instance == nullptr)
	{
		LOG_ERROR("Could not create an instance of MonsterChase!");
		return false;
	}

	// initialize the game
	bool success = mc_instance->Init();
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
	monsters_.clear();

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

	// create the player
	CreatePlayer();
	LOG("Created the player...");

	// create the monsters
	engine::gameobject::ActorCreator::CreateActorsFromFile(GameData::MONSTERS_LUA_FILE_NAME, monsters_);

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
	engine::util::FileUtils::Get()->ReadFile(GameData::PLAYER_TEXTURE_NAME);
	engine::util::FileUtils::Get()->ReadFile(GameData::SILLY_MONSTER_TEXTURE_NAME);
	engine::util::FileUtils::Get()->ReadFile(GameData::SMART_MONSTER_TEXTURE_NAME);
	return true;
}

void Game::Update(float dt)
{
	bool quit = false;
	GLib::Service(quit);
	
	// request the engine to quit if we need to
	if (quit || game_state_ == GameStates::kGameStateQuit)
	{
		engine::RequestQuit();
	}
}

void Game::OnKeyPressed(unsigned int i_key_id)
{
	game_state_ = i_key_id == 'Q' ? GameStates::kGameStateQuit : game_state_;
}

void Game::CreatePlayer()
{
	// create the player at the center of the grid
	player_ = new Player();
}

} // namespace monsterchase
