#pragma once
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
	void SaveNumMonsters(int num_monsters);
	void CreateMonster(const char* input_name = NULL);
	void DestroyMonster(int at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* name);

	void CreatePlayer(const char* name);

	inline GameState GetState() { return game_state_; }

	// game constants
	static const int MAX_INPUT_SIZE = 256;
	static const int MAX_ROWS = 100;
	static const int MAX_COLS = 100;
	static const int START_ASCII = 97;
	static const int MAX_ASCII = 25;

private:
	// field to maintain the current state of the game
	GameState game_state_;

	// game elements
	engine::Player* player_;
	Monster** monsters_;

	// game counters
	int initial_num_monsters_;
	int num_monsters_;
	int ascii_index_;
};

