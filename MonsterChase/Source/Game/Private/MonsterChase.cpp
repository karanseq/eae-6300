#include "Game\MonsterChase.h"
#include "Game\Player.h"
#include "Game\Monster.h"
#include "Game\GameUtils.h"

// engine includes 
#include "Math\Vec2D.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorOverrides.h"

// library includes
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

// static member initialization
engine::BlockAllocator* MonsterChase::game_allocator_ = nullptr;

MonsterChase::MonsterChase()
{
	// allocate memory for the game objects
	void* aligned_memory = engine::BlockAllocator::CreateDefaultAllocator()->Alloc(MEMORY_SIZE);
	ASSERT(aligned_memory);

	// create an allocator to manage memory for the game objects
	MonsterChase::game_allocator_ = engine::BlockAllocator::Create(aligned_memory, MEMORY_SIZE);
	ASSERT(MonsterChase::game_allocator_);

	game_state_ = GameStates::kGameStateBegin;
	player_ = nullptr;
	monsters_ = nullptr;
	num_monsters_ = 0;
	ascii_index_ = 0;
	srand((unsigned int)time(0));
}

MonsterChase::~MonsterChase()
{
	// deallocate the player
	SAFE_DELETE(player_);

	// deallocate the monsters
	for (int i = 0; i < num_monsters_; ++i)
	{
		SAFE_DELETE(monsters_[i]);
	}
	SAFE_DELETE_ARRAY(monsters_);

	// deallocate the allocator
	engine::BlockAllocator::Destroy(MonsterChase::game_allocator_);
	engine::BlockAllocator::CreateDefaultAllocator()->Free(MonsterChase::game_allocator_);
}

void MonsterChase::Update()
{
	// first display game information
	PrintMessage();

	// then wait for input
	AcceptInput();
}

void MonsterChase::PrintMessage(const char* message)
{
	printf(message);
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

	char message[256] = { 0 };
	sprintf_s(message, 256, "What would you like to name Monster-%d? ", num_monsters_ + 1);
	PrintMessage(message);
}

void MonsterChase::PrintGameInformation()
{
	for (int i = 0; i < num_monsters_; ++i)
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

void MonsterChase::ValidateInput(const char* input)
{
	switch (game_state_)
	{
	case GameStates::kGameStateInputNumMonsters:
		ValidateNumber(input);
		break;

	case GameStates::kGameStateInputMonsterNames:
	case GameStates::kGameStateInputPlayerName:
		ValidateName(input);
		break;

	case GameStates::kGameStateRunning:
		ValidateMove(input);
		break;
	}
}

void MonsterChase::ValidateNumber(const char* input)
{
	// valide input
	ASSERT(input != nullptr);

	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateInputNumMonsters)
	{
		return;
	}

	// check if a number was entered
	int number = -1;
	if (sscanf_s(input, "%d", &number) > 0)
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

void MonsterChase::ValidateName(const char* input)
{
	// validate input
	ASSERT(input != nullptr);

	// execute this function only in these states
	if (game_state_ != GameStates::kGameStateInputMonsterNames && game_state_ != GameStates::kGameStateInputPlayerName)
	{
		return;
	}

	// count the number of white spaces in the input
	char c = 0;
	int i = 0, count = 0;
	while (input[i])
	{
		c = input[i++];
		count += isblank(c) ? 1 : 0;
	}

	int input_length = static_cast<int>(strlen(input));
	if (input_length > MAX_NAME_LENGTH						// check if the name was within our range
		|| count >= input_length - 1)						// check if the input contained only white spaces
	{
		// print an error message
		char buf[256] = { 0 };
		sprintf_s(buf, 256, "Please enter a name that is 1 to %d characters long.\n", MAX_NAME_LENGTH);
		PrintMessage(buf);
		return;
	}

	// remove the newline character from the input
	char name[MAX_NAME_LENGTH] = { 0 };
	strncpy_s(name, input, strlen(input) - 1);

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

void MonsterChase::ValidateMove(const char* input)
{
	// validate input
	ASSERT(input != nullptr);

	// execute this function only in this state
	if (game_state_ != GameStates::kGameStateRunning)
	{
		return;
	}

	bool is_valid_input = false;
	char move = input[0];
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

void MonsterChase::SaveNumMonsters(int num_monsters)
{
	// validate input
	ASSERT(num_monsters > 0);

	// save the number of monsters
	initial_num_monsters_ = num_monsters;

	// create the array
	monsters_ = new (MonsterChase::game_allocator_) Monster*[MAX_MONSTERS];

	// time to query the names of each monster
	game_state_ = GameStates::kGameStateInputMonsterNames;
}

void MonsterChase::CreateMonster(const char* input_name)
{
	// limit number of monsters
	if (num_monsters_ >= MAX_MONSTERS)
	{
		LOG("Already at max monsters!");
		return;
	}

	char name[MAX_INPUT_SIZE] = { 0 };

	// if no name was provided, generate one
	if (input_name == nullptr)
	{
		GetNameForMonster(name);
	}
	else
	{
		sprintf_s(name, "%s", input_name);
	}

	// calculate random position for this monster
	engine::Vec3D monster_position = GameUtils::GetRandomVec3D(MAX_ROWS, MAX_COLS);
	monster_position *= (rand() % 10) > 5 ? 1.0f : -1.0f;

	MonsterControllers controller_type = (rand() % 10) > 5 ? MonsterControllers::kSmartMonsterController : MonsterControllers::kSillyMonsterController;

	// create a new monster at the back of the array
	Monster* monster = new (MonsterChase::game_allocator_) Monster(controller_type);
	monsters_[num_monsters_] = monster;
	++num_monsters_;

	// set this monster's attributes
	monster->GetController()->GetGameObject()->SetPosition(monster_position);
	monster->GetIdentity()->SetName(name);
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

void MonsterChase::DestroyMonster(int at_index)
{
	// validate inputs, bounds and data state
	ASSERT(at_index >= 0);
	ASSERT(at_index < num_monsters_);
	ASSERT(num_monsters_ != 0);

	// delete the monster at index
	SAFE_DELETE(monsters_[at_index]);

	// check if this monster is the last one
	if (at_index != num_monsters_ - 1)
	{
		// TODO: move last object and reduce fragmentation

		// swap the last monster to occupy this index
		monsters_[at_index] = monsters_[num_monsters_ - 1];

		// clear the pointer
		monsters_[num_monsters_ - 1] = nullptr;
	}

	// reduce number of monsters
	--num_monsters_;
}

void MonsterChase::UpdateMonsters()
{
	// touch all monsters
	for (int i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Update();
	}

	// check if any monsters must be deleted
	for (int i = 0; i < num_monsters_; ++i)
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

void MonsterChase::GetNameForMonster(char* name)
{
	// validate input
	ASSERT(name != nullptr);

	// generate a name based by appending an incrementing ASCII value
	sprintf_s(name, MAX_INPUT_SIZE, "foo%c", (START_ASCII + ascii_index_++));
	if (ascii_index_ > MAX_ASCII)
	{
		ascii_index_ = 0;
	}
}

void MonsterChase::CreatePlayer(const char* name)
{
	// validate inputs
	ASSERT(name != nullptr);

	// create the player at the center of the grid
	player_ = new (MonsterChase::game_allocator_) Player();
	player_->GetIdentity()->SetName(name);

	// time to start the game
	game_state_ = GameStates::kGameStateInputNumMonsters;
}
