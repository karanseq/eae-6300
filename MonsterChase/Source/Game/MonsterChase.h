#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

#include "Game\GameTypes.h"

// forward declarations
class Player;
class Monster;

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

	inline GameStates GetState() const { return game_state_; }

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
	GameStates game_state_;

	// game elements
	Player* player_;
	Monster** monsters_;

	// game counters
	int initial_num_monsters_;
	int num_monsters_;
	int ascii_index_;
};

#endif // MONSTER_CHASE_H_