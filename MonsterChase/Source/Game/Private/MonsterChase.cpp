#include "Game\MonsterChase.h"

// engine includes 
#include "Assert\Assert.h"
#include "Common\Engine.h"
#include "GLib.h"
#include "Logger\Logger.h"
#include "Math\Vec2D.h"
#include "Memory\AllocatorOverrides.h"
#include "Memory\BlockAllocator.h"
#include "Time\Updater.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\Monster.h"
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

	if (i_went_down)
	{
		MonsterChase::GetInstance()->ValidateInput(i_key_id);
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
engine::memory::BlockAllocator* MonsterChase::game_allocator_ = nullptr;

MonsterChase* MonsterChase::Create()
{
	if (!MonsterChase::instance_)
	{
		MonsterChase::instance_ = new MonsterChase();
	}
	return MonsterChase::instance_;
}

void MonsterChase::Destroy()
{
	SAFE_DELETE(MonsterChase::instance_);
}

MonsterChase::MonsterChase() : game_state_(GameStates::kGameStateBegin),
	player_(nullptr),
	num_monsters_(0),
	ascii_index_(0)
{
	// allocate memory for the game objects
	void* aligned_memory = engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(MEMORY_SIZE);
	ASSERT(aligned_memory);

	// create an allocator to manage memory for the game objects
	MonsterChase::game_allocator_ = engine::memory::BlockAllocator::Create(aligned_memory, MEMORY_SIZE);
	ASSERT(MonsterChase::game_allocator_);

	// init game data
	GameData::Create();

	// register the key callback
	GLib::SetKeyStateChangeCallback(monsterchase::AcceptKey);

	// register the allocator
	engine::memory::BlockAllocator::AddBlockAllocator(MonsterChase::game_allocator_);

	// reserve memory for the monster pointers
	//monsters_.reserve(MAX_MONSTERS);
}

MonsterChase::~MonsterChase()
{
	// tell the engine we no longer want to be ticked
	engine::time::Updater::Get()->RemoveTickable(this);

	// deallocate the player
	SAFE_DELETE(player_);

	// deallocate the monsters
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		SAFE_DELETE(monsters_[i]);
	}
	monsters_.clear();

	// delete game data
	GameData::Destroy();

	// deregister the allocator
	engine::memory::BlockAllocator::RemoveBlockAllocator(MonsterChase::game_allocator_);

	// deallocate the allocator
	engine::memory::BlockAllocator::Destroy(MonsterChase::game_allocator_);
	engine::memory::BlockAllocator::GetDefaultAllocator()->Free(MonsterChase::game_allocator_);
	MonsterChase::game_allocator_ = nullptr;
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
	CreatePlayer("player");
	LOG("Created the player...");

	// create the monsters
	/*const uint8_t num_monsters = (MAX_MONSTERS / 2) + rand() % (MAX_MONSTERS / 2);
	for (uint8_t i = 0; i < num_monsters; ++i)
	{
		CreateMonster();
	}
	LOG("Created %d monsters...",  num_monsters_);*/

	// tell the engine we want to be ticked
	engine::time::Updater::Get()->AddTickable(this);

	game_state_ = GameStates::kGameStateRunning;

	return true;
}

void MonsterChase::Update(float dt)
{
	bool quit = false;
	GLib::Service(quit);

	if (!quit)
	{
		Render();
	}
	
	// request the engine to quit if we need to
	if (quit || game_state_ == GameStates::kGameStateQuit)
	{
		engine::RequestQuit();
	}
}

void MonsterChase::Render()
{
	// Tell GLib that we want to start rendering
	GLib::BeginRendering();
	// Tell GLib that we want to render some sprites
	GLib::Sprites::BeginRendering();

	player_->Render();
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Render();
	}

	// Tell GLib we're done rendering sprites
	GLib::Sprites::EndRendering();
	// Tell GLib we're done rendering
	GLib::EndRendering();
}

void MonsterChase::PrintGameInformation()
{
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Print();
	}

	player_->Print();
}

void MonsterChase::ValidateInput(uint8_t i_input)
{
	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateRunning)
	{
		return;
	}

	bool is_valid_input = false;
	if (i_input == static_cast<uint8_t>(KeyboardKeys::kQ))
	{
		game_state_ = GameStates::kGameStateQuit;
		return;
	}

	if (i_input == static_cast<uint8_t>(KeyboardKeys::kM))
	{
		is_valid_input = true;
		CreateMonster();
	}

	is_valid_input = player_->HandleUserInput(static_cast<KeyboardKeys>(i_input));

	if (is_valid_input)
	{
		player_->Update();
		UpdateMonsters();

		PrintGameInformation();
	}
}

bool MonsterChase::LoadGameData()
{
	// load textures
	GameUtils::LoadFile(GameData::PLAYER_TEXTURE_NAME);
	GameUtils::LoadFile(GameData::SILLY_MONSTER_TEXTURE_NAME);
	GameUtils::LoadFile(GameData::SMART_MONSTER_TEXTURE_NAME);
	return true;
}

void MonsterChase::CreateMonster(const char* i_input_name)
{
	// limit number of monsters
	if (num_monsters_ >= MAX_MONSTERS)
	{
		LOG("Already at max monsters!");
		return;
	}

	char name[MAX_INPUT_SIZE] = { 0 };

	// if no name was provided, generate one
	if (i_input_name == nullptr)
	{
		GetNameForMonster(name);
	}
	else
	{
		sprintf_s(name, "%s", i_input_name);
	}

	// calculate random position for this monster
	engine::math::Vec3D monster_position = GameUtils::GetRandomVec3D(MAX_COLS, MAX_ROWS);
	monster_position *= (rand() % 10) > 5 ? 1.0f : -1.0f;

	MonsterControllers controller_type = (rand() % 10) > 5 ? MonsterControllers::kSmartMonsterController : MonsterControllers::kSillyMonsterController;

	// create a new monster at the back of the array
	Monster* monster = new (MonsterChase::game_allocator_) Monster(controller_type, name);
	monsters_.push_back(monster);
	++num_monsters_;

	// set this monster's attributes
	monster->GetController()->GetGameObject()->SetPosition(monster_position);
	monster->SetTimeToLive(MAX_MONSTER_TTL / 2 + rand() % MAX_MONSTER_TTL);

	// handle smart monsters differently
	if (controller_type == MonsterControllers::kSmartMonsterController)
	{
		reinterpret_cast<SmartMonsterController*>(monster->GetController())->SetTarget(player_->GetController()->GetGameObject());
	}
}

void MonsterChase::DestroyMonster(uint8_t i_at_index)
{
	// validate inputs, bounds and data state
	ASSERT(i_at_index >= 0);
	ASSERT(i_at_index < num_monsters_);
	ASSERT(num_monsters_ != 0);

	// swap this monster with the last one
	std::swap(monsters_[i_at_index], monsters_.back());

	// delete this monster
	SAFE_DELETE(monsters_.back());
	// remove its pointer from the array
	monsters_.pop_back();

	// reduce number of monsters
	--num_monsters_;
}

void MonsterChase::UpdateMonsters()
{
	// touch all monsters
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Update();
	}

	// check if any monsters must be deleted
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		if (monsters_[i]->GetTimeToLive() <= 0)
		{
			// delete this monster
			DestroyMonster(i);

			// the function above swaps the monsters
			// decrement the index to preocess the monster occupying this place after the swap
			--i;
		}
	}
}

void MonsterChase::GetNameForMonster(char* o_name)
{
	// validate input
	ASSERT(o_name != nullptr);

	// generate a name based by appending an incrementing ASCII value
	sprintf_s(o_name, MAX_INPUT_SIZE, "foo%c", (START_ASCII + ascii_index_++));
	if (ascii_index_ > MAX_ASCII)
	{
		ascii_index_ = 0;
	}
}

void MonsterChase::CreatePlayer(const char* i_name)
{
	// validate inputs
	ASSERT(i_name != nullptr);

	// create the player at the center of the grid
	player_ = new (MonsterChase::game_allocator_) Player(i_name);
}

} // namespace monsterchase
