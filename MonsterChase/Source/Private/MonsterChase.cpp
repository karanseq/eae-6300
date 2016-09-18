#include "MonsterChase.h"

// include libraries
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

MonsterChase::MonsterChase()
{
	game_state_ = GameStateBegin;
	player_ = NULL;
	monsters_ = NULL;
	num_monsters_ = 0;
	ascii_index_ = 0;
	srand((unsigned int)time(0));
}

MonsterChase::~MonsterChase()
{
	// deallocate the player
	if (player_ != NULL)
	{
		delete player_;
		player_ = NULL;
	}

	// deallocate the monsters
	if (monsters_ != NULL)
	{
		for (int i = 0; i < num_monsters_; ++i)
		{
			if (monsters_[i] != NULL)
			{
				delete monsters_[i];
				monsters_[i] = NULL;
			}
		}
		delete[] monsters_;
		monsters_ = NULL;
	}
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
	case GameStateBegin:
		PrintMessage("Welcome to MonsterChase!\n\n");
		game_state_ = GameStateInputNumMonsters;
		PrintMessage();
		break;

	case GameStateInputNumMonsters:
		PrintMessage("How many monsters would you like to start with? ");
		break;

	case GameStateInputMonsterNames:
		PrintMessageMonsterName();
		break;

	case GameStateInputPlayerName:
		PrintMessage("What would you like to name the player? ");
		break;

	case GameStateRunning:
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
	if (game_state_ != GameStateInputMonsterNames)
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
	if (game_state_ == GameStateRunning)
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
	case GameStateInputNumMonsters:
		ValidateNumber(input);
		break;

	case GameStateInputMonsterNames:
	case GameStateInputPlayerName:
		ValidateName(input);
		break;

	case GameStateRunning:
		ValidateMove(input);
		break;
	}
}

void MonsterChase::ValidateNumber(const char* input)
{
	// valide input
	if (input == NULL)
	{
		return;
	}

	// execute this function only in this state
	if (game_state_ != GameStateInputNumMonsters)
	{
		return;
	}

	// check if a number was entered
	int number = -1;
	if (sscanf_s(input, "%d", &number) > 0)
	{
		// check if the number was within our range
		if (number > 0 && number <= Monster::MAX_MONSTERS)
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
	if (input == NULL)
	{
		return;
	}

	// execute this function only in these states
	if (game_state_ != GameStateInputMonsterNames && game_state_ != GameStateInputPlayerName)
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

	int input_length = (int)strlen(input);
	if (input_length > Player::MAX_NAME_LENGTH				// check if the name was within our range
		|| count >= input_length - 1)						// check if the input contained only white spaces
	{
		// print an error message
		char buf[256] = { 0 };
		sprintf_s(buf, 256, "Please enter a name that is 1 to %d characters long.\n", Player::MAX_NAME_LENGTH);
		PrintMessage(buf);
		return;
	}

	// remove the newline character from the input
	char name[Player::MAX_NAME_LENGTH] = { 0 };
	strncpy_s(name, input, strlen(input) - 1);

	// handle both states differently
	if (game_state_ == GameStateInputMonsterNames)
	{
		CreateMonster(name);
	}
	else if (game_state_ == GameStateInputPlayerName)
	{
		CreatePlayer(name);
	}
}

void MonsterChase::ValidateMove(const char* input)
{
	// validate input
	if (input == NULL)
	{
		return;
	}

	// execute this function only in this state
	if (game_state_ != GameStateRunning)
	{
		return;
	}

	bool is_valid_input = false;
	char move = input[0];
	if (move == 'q' || move == 'Q')
	{
		game_state_ = GameStateQuit;
		return;
	}

	if (move == 'a' || move == 'A')
	{
		is_valid_input = true;
		player_->Move(MoveDirectionLeft);
	}
	else if (move == 'd' || move == 'D')
	{
		is_valid_input = true;
		player_->Move(MoveDirectionRight);
	}
	else if (move == 'w' || move == 'W')
	{
		is_valid_input = true;
		player_->Move(MoveDirectionUp);
	}
	else if (move == 's' || move == 'S')
	{
		is_valid_input = true;
		player_->Move(MoveDirectionDown);
	}
	else if (move == 'm' || move == 'M')
	{
		is_valid_input = true;
		CreateMonster();
	}

	if (is_valid_input)
	{
		UpdateMonsters();
	}
}

void MonsterChase::SaveNumMonsters(int num_monsters)
{
	// validate input
	if (num_monsters <= 0)
	{
		return;
	}

	// save the number of monsters
	initial_num_monsters_ = num_monsters;

	// create the array
	monsters_ = new Monster*[Monster::MAX_MONSTERS];

	// time to query the names of each monster
	game_state_ = GameStateInputMonsterNames;
}

void MonsterChase::CreateMonster(const char* input_name)
{
	// limit number of monsters
	if (num_monsters_ >= Monster::MAX_MONSTERS)
	{
		return;
	}

	char name[MAX_INPUT_SIZE] = { 0 };

	// if no name was provided, generate one
	if (input_name == NULL)
	{
		GetNameForMonster(name);
	}
	else
	{
		sprintf_s(name, "%s", input_name);
	}

	// calculate random position for this monster
	engine::Vec2D monster_position = engine::Vec2D((float)(rand() % MAX_ROWS), (float)(rand() % MAX_COLS));
	monster_position *= (rand() % 10) > 5 ? 1.0f : -1.0f;

	// create a new monster at the back of the array
	monsters_[num_monsters_++] = new Monster(monster_position, name);

	// check if all monsters have been created
	if (initial_num_monsters_ != -1 && num_monsters_ >= initial_num_monsters_)
	{
		// enter this loop only when creating the monsters for the first time
		initial_num_monsters_ = -1;

		// now its the player's turn
		game_state_ = GameStateInputPlayerName;
	}
}

void MonsterChase::DestroyMonster(int at_index)
{
	// validate inputs, bounds and data state
	if (at_index < 0 || at_index >= num_monsters_ || num_monsters_ == 0)
	{
		return;
	}

	// delete the monster at index
	delete monsters_[at_index];
	monsters_[at_index] = NULL;

	// check if this monster is the last one
	if (at_index != num_monsters_ - 1)
	{
		// TODO: move last object and reduce fragmentation

		// swap the last monster to occupy this index
		monsters_[at_index] = monsters_[num_monsters_ - 1];

		// clear the pointer
		monsters_[num_monsters_ - 1] = NULL;
	}

	// reduce number of monsters
	--num_monsters_;
}

void MonsterChase::UpdateMonsters()
{
	// touch all monsters
	for (int i = 0; i < num_monsters_; ++i)
	{
		monsters_[i]->Move(player_->GetPosition());
	}

	// check if any monsters must be deleted
	for (int i = 0; i < num_monsters_; ++i)
	{
		if (monsters_[i]->GetTimeToLive() <= 0)
		{
			// delete this monster
			DestroyMonster(i);

			// the function above swaps the monsters
			// decrement the index to process the monster occupying this place after the swap 
			--i;
		}
	}
}	

void MonsterChase::GetNameForMonster(char* name)
{
	// validate input
	if (name == NULL)
	{
		return;
	}

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
	if (name == NULL)
	{
		return;
	}

	// create the player at the center of the grid
	player_ = new Player(engine::Vec2D::ZERO, name);

	// time to start the game
	game_state_ = GameStateRunning;
}
