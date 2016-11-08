#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

#include "Player.h"
#include "Monster.h"

#include "PlayerController.h"
#include "MonsterController.h"

// An enum to maintain the various states of the game
enum GameState
{
	GameStateNone = 0,
	GameStateBegin,
	GameStateInputPlayerName,
	GameStateInputNumMonsters,
	GameStateInputMonsterNames,
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
	void CreateMonster(const char* input_name = nullptr);
	void DestroyMonster(int at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* name);

	void CreatePlayer(const char* name);

	inline GameState GetState() const { return game_state_; }

	// game constants
	static const int MAX_INPUT_SIZE = 256;
	static const int MAX_ROWS = 50;
	static const int MAX_COLS = 50;
	static const int START_ASCII = 97;
	static const int MAX_ASCII = 25;
	static const int MAX_MONSTERS = 10;
	static const int MAX_MONSTER_TTL = 10;
	static const int MAX_NAME_LENGTH = 10;

private:
	// field to maintain the current state of the game
	GameState game_state_;

	// game elements
	PlayerController* player_controller_;
	MonsterController** monster_controllers_;

	// game counters
	int initial_num_monsters_;
	int num_monsters_;
	int ascii_index_;
};

#endif // MONSTER_CHASE_H_