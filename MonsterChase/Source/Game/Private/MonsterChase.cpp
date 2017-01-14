#include "Game\MonsterChase.h"

// library includes
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// engine includes 
#include "Assert\Assert.h"
#include "GLib.h"
#include "Logger\Logger.h"
#include "Math\Vec2D.h"
#include "Memory\AllocatorOverrides.h"
#include "Memory\BlockAllocator.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\Monster.h"
#include "Game\Player.h"

namespace monsterchase {

// TODO: Find a better place for this
void AcceptKey(unsigned int i_key_id, bool i_went_down)
{
#ifdef BUILD_DEBUG
	const size_t		buffer_size = 65;
	char				buffer[buffer_size];

	sprintf_s(buffer, buffer_size, "Key 0x%04x went %s\n", i_key_id, i_went_down ? "down" : "up");
	LOG(buffer);
#endif
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
	player_(nullptr)
{
	// allocate memory for the game objects
	void* aligned_memory = engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(MEMORY_SIZE);
	ASSERT(aligned_memory);

	monsters_.reserve(MAX_MONSTERS);

	// create an allocator to manage memory for the game objects
	MonsterChase::game_allocator_ = engine::memory::BlockAllocator::Create(aligned_memory, MEMORY_SIZE);
	ASSERT(MonsterChase::game_allocator_);

	// register the key callback
	GLib::SetKeyStateChangeCallback(monsterchase::AcceptKey);

	// register the allocator
	engine::memory::BlockAllocator::AddBlockAllocator(MonsterChase::game_allocator_);

	srand(static_cast<unsigned int>(time(0)));
}

MonsterChase::~MonsterChase()
{
	// deallocate the player
	SAFE_DELETE(player_);

	// deallocate the monsters
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		SAFE_DELETE(monsters_[i]);
	}
	monsters_.clear();

	// deregister the allocator
	engine::memory::BlockAllocator::RemoveBlockAllocator(MonsterChase::game_allocator_);

	// deallocate the allocator
	engine::memory::BlockAllocator::Destroy(MonsterChase::game_allocator_);
	engine::memory::BlockAllocator::GetDefaultAllocator()->Free(MonsterChase::game_allocator_);
	MonsterChase::game_allocator_ = nullptr;
}

void MonsterChase::Update()
{
	/*// first display game information
	PrintMessage();

	// then wait for input
	AcceptInput();*/

	// TODO: change this to a dynamic value
	bool quit = false;
	GLib::Service(quit);
}

void MonsterChase::PrintMessage(const char* i_message)
{
	printf(i_message);
}

void MonsterChase::PrintMessage()
{
	switch (game_state_)
	{
	case GameStates::kGameStateBegin:
		PrintMessage("Welcome to MonsterChase!\n\n");
		game_state_ = GameStates::kGameStateInputPlayerName;
		PrintMessage();
		break;

	case GameStates::kGameStateInputPlayerName:
		PrintMessage("What would you like to name the player? ");
		break;

	case GameStates::kGameStateInputNumMonsters:
		PrintMessage("How many monsters would you like to start with? ");
		break;

	case GameStates::kGameStateInputMonsterNames:
		PrintMessageMonsterName();
		break;

	case GameStates::kGameStateRunning:
		PrintGameInformation();
		PrintMessage("\nPress A to move left, D to move right, W to move up, S to move down, M to spawn a new monster and Q to quit.\n");
		break;

	default:
		PrintMessage("MonsterChase has reached an undefined state... :-(\nPlease press Q to exit.");
		break;
	}
}

void MonsterChase::PrintMessageMonsterName()
{
	if (game_state_ != GameStates::kGameStateInputMonsterNames)
	{
		return;
	}

	const size_t		message_size = 256;
	char				message[message_size] = { 0 };
	sprintf_s(message, message_size, "What would you like to name Monster-%d? ", num_monsters_ + 1);
	PrintMessage(message);
}

void MonsterChase::PrintGameInformation()
{
	for (uint8_t i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Print();
	}

	player_->Print();
}

void MonsterChase::AcceptInput()
{
	char input_str[MAX_INPUT_SIZE + 1] = { 0 };
	if (game_state_ == GameStates::kGameStateRunning)
	{
		input_str[0] = _getch();
	}
	else
	{
		fgets(input_str, sizeof(input_str), stdin);
	}
	ValidateInput(input_str);
}

void MonsterChase::ValidateInput(const char* i_input)
{
	switch (game_state_)
	{
	case GameStates::kGameStateInputNumMonsters:
		ValidateNumber(i_input);
		break;

	case GameStates::kGameStateInputMonsterNames:
	case GameStates::kGameStateInputPlayerName:
		ValidateName(i_input);
		break;

	case GameStates::kGameStateRunning:
		ValidateMove(i_input);
		break;
	}
}

void MonsterChase::ValidateNumber(const char* i_input)
{
	// valide input
	ASSERT(i_input != nullptr);

	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateInputNumMonsters)
	{
		return;
	}

	// check if a number was entered
	int number = -1;
	if (sscanf_s(i_input, "%d", &number) > 0)
	{
		// check if the number was within our range
		if (number > 0 && number <= MAX_MONSTERS)
		{
			SaveNumMonsters(number);
			return;
		}
	}

	PrintMessage("Please pick a number from 1 to 10.\n");
}

void MonsterChase::ValidateName(const char* i_input)
{
	// validate input
	ASSERT(i_input != nullptr);

	// execute this function only in these states
	if (game_state_ != GameStates::kGameStateInputMonsterNames && game_state_ != GameStates::kGameStateInputPlayerName)
	{
		return;
	}

	// count the number of white spaces in the input
	char		c = 0;
	uint8_t		i = 0, count = 0;
	while (i_input[i])
	{
		c = i_input[i++];
		count += isblank(c) ? 1 : 0;
	}

	uint8_t input_length = static_cast<uint8_t>(strlen(i_input));
	if (input_length > MAX_NAME_LENGTH						// check if the name was within our range
		|| count >= input_length - 1)						// check if the input contained only white spaces
	{
		// print an error message
		const size_t message_size = 256;
		char buf[message_size] = { 0 };
		sprintf_s(buf, message_size, "Please enter a name that is 1 to %d characters long.\n", MAX_NAME_LENGTH);
		PrintMessage(buf);
		return;
	}

	// remove the newline character from the input
	char name[MAX_NAME_LENGTH] = { 0 };
	strncpy_s(name, i_input, strlen(i_input) - 1);

	// handle both states differently
	if (game_state_ == GameStates::kGameStateInputPlayerName)
	{
		CreatePlayer(name);
	}
	else if (game_state_ == GameStates::kGameStateInputMonsterNames)
	{
		CreateMonster(name);
	}
}

void MonsterChase::ValidateMove(const char* i_input)
{
	// validate input
	ASSERT(i_input != nullptr);

	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateRunning)
	{
		return;
	}

	bool is_valid_input = false;
	char move = i_input[0];
	if (move == 'q' || move == 'Q')
	{
		game_state_ = GameStates::kGameStateQuit;
		return;
	}

	if (move == 'm' || move == 'M')
	{
		is_valid_input = true;
		CreateMonster();
	}

	is_valid_input = player_->HandleUserInput(move);

	if (is_valid_input)
	{
		player_->Update();
		UpdateMonsters();
	}
}

void MonsterChase::SaveNumMonsters(uint8_t i_num_monsters)
{
	// validate input
	ASSERT(i_num_monsters > 0);

	// save the number of monsters
	initial_num_monsters_ = i_num_monsters;

	// time to query the names of each monster
	game_state_ = GameStates::kGameStateInputMonsterNames;
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
	engine::math::Vec3D monster_position = GameUtils::GetRandomVec3D(MAX_ROWS, MAX_COLS);
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

	// check if all monsters have been created
	if (initial_num_monsters_ != -1 && num_monsters_ >= initial_num_monsters_)
	{
		// enter this loop only when creating the monsters for the first time
		initial_num_monsters_ = -1;

		// now its the player's turn
		game_state_ = GameStates::kGameStateRunning;
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

	// time to start the game
	game_state_ = GameStates::kGameStateInputNumMonsters;
}

} // namespace monsterchase
