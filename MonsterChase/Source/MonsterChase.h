#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

#include <stdint.h>

#include "Player.h"
#include "Monster.h"

// An enum to maintain the various states of the game
enum GameState
{
	GameStateNone = 0,
	GameStateBegin,
	GameStateInputNumMonsters,
	GameStateInputMonsterNames,
	GameStateInputPlayerName,
	GameStateRunning,
	GameStateQuit
};

// An enum to describe the direction of movement
enum MoveDirection
{
	MoveDirectionLeft = 0,
	MoveDirectionRight,
	MoveDirectionUp,
	MoveDirectionDown
};

class MonsterChase
{
public:
	MonsterChase();
	~MonsterChase();

	// the main game loop
	// called every tick from main
	void Update();

	// functions to print game information
	void PrintMessage(const char* message);
	void PrintMessage();
	void PrintMessageMonsterName();	
	void PrintGameInformation();

	// functions to scan and validate user input
	void AcceptInput();
	void ValidateInput(const char* input);
	void ValidateNumber(const char* input);
	void ValidateName(const char* input);
	void ValidateMove(const char* input);

	// game logic
	void SaveNumMonsters(uint8_t num_monsters);
	void CreateMonster(const char* input_name = NULL);
	void DestroyMonster(uint8_t at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* name);

	void CreatePlayer(const char* name);

	inline GameState GetState() const { return game_state_; }

	// game constants
	static const uint16_t MAX_INPUT_SIZE = 256;
	static const uint8_t MAX_ROWS = 50;
	static const uint8_t MAX_COLS = 50;
	static const uint8_t START_ASCII = 97;
	static const uint8_t MAX_ASCII = 25;

private:
	// field to maintain the current state of the game
	GameState game_state_;

	// game elements
	Player* player_;
	Monster** monsters_;

	// game counters
	uint8_t initial_num_monsters_;
	uint8_t num_monsters_;
	uint8_t ascii_index_;
};

#endif // MONSTER_CHASE_H_