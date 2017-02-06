#include "Game\MonsterChase.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "Common\HelperMacros.h"
#include "GLib.h"
#include "Logger\Logger.h"
#include "Time\Updater.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameData.h"
#include "Game\GameUtils.h"
#include "Game\Player.h"

namespace monsterchase {

// TODO: Find a better place/scope for this
void AcceptKey(unsigned int i_key_id, bool i_went_down)
{
#ifdef BUILD_DEBUG
	const size_t		buffer_size = 65;
	char				buffer[buffer_size];

	sprintf_s(buffer, buffer_size, "Key 0x%04x | %d went %s\n", i_key_id, i_key_id, i_went_down ? "down" : "up");
	LOG(buffer);
#endif

	switch (static_cast<KeyboardKeys>(i_key_id))
	{
	case KeyboardKeys::kA:
		MonsterChase::KEY_A_PRESSED = i_went_down;
		break;
	case KeyboardKeys::kD:
		MonsterChase::KEY_D_PRESSED = i_went_down;
		break;
	case KeyboardKeys::kM:
		MonsterChase::KEY_M_PRESSED = i_went_down;
		break;
	case KeyboardKeys::kQ:
		MonsterChase::KEY_Q_PRESSED = i_went_down;
		break;
	case KeyboardKeys::kS:
		MonsterChase::KEY_S_PRESSED = i_went_down;
		break;
	case KeyboardKeys::kW:
		MonsterChase::KEY_W_PRESSED = i_went_down;
		break;
	}
}

bool StartUp()
{
	// create an instance of the game
	MonsterChase* mc_instance = MonsterChase::Create();
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
	MonsterChase::Destroy();
	LOG("-------------------- MonsterChase Shutdown --------------------");
}

// static member initialization
MonsterChase* MonsterChase::instance_ = nullptr;

bool MonsterChase::KEY_A_PRESSED = false;
bool MonsterChase::KEY_D_PRESSED = false;
bool MonsterChase::KEY_M_PRESSED = false;
bool MonsterChase::KEY_Q_PRESSED = false;
bool MonsterChase::KEY_S_PRESSED = false;
bool MonsterChase::KEY_W_PRESSED = false;

MonsterChase* MonsterChase::Create()
{
	// create the singleton instance of MonsterChase
	if (!MonsterChase::instance_)
	{
		MonsterChase::instance_ = new MonsterChase();
	}
	return MonsterChase::instance_;
}

void MonsterChase::Destroy()
{
	// destroy the singleton instance of MonsterChase
	SAFE_DELETE(MonsterChase::instance_);
}

MonsterChase::MonsterChase() : game_state_(GameStates::kGameStateBegin),
	player_(nullptr)
{
	// register the key callback
	GLib::SetKeyStateChangeCallback(monsterchase::AcceptKey);
}

MonsterChase::~MonsterChase()
{
	// delete the player
	SAFE_DELETE(player_);

	// tell the engine we no longer want to be ticked
	engine::time::Updater::Get()->RemoveTickable(this);
}

bool MonsterChase::Init()
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

	// tell the engine we want to be ticked
	engine::time::Updater::Get()->AddTickable(this);

	game_state_ = GameStates::kGameStateRunning;

	return true;
}

void MonsterChase::Update(float dt)
{
	bool quit = false;
	GLib::Service(quit);

	CheckInput();
	
	// request the engine to quit if we need to
	if (quit || game_state_ == GameStates::kGameStateQuit)
	{
		engine::RequestQuit();
	}
}

void MonsterChase::CheckInput()
{
	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateRunning)
	{
		return;
	}

	if (MonsterChase::KEY_Q_PRESSED)
	{
		KEY_Q_PRESSED = false;
		game_state_ = GameStates::kGameStateQuit;
		return;
	}
}

bool MonsterChase::LoadGameData()
{
	// load textures
	engine::util::FileUtils::Get()->ReadFile(GameData::PLAYER_TEXTURE_NAME);
	engine::util::FileUtils::Get()->ReadFile(GameData::SILLY_MONSTER_TEXTURE_NAME);
	engine::util::FileUtils::Get()->ReadFile(GameData::SMART_MONSTER_TEXTURE_NAME);
	return true;
}

void MonsterChase::CreatePlayer()
{
	// create the player at the center of the grid
	player_ = new Player();
}

} // namespace monsterchase
