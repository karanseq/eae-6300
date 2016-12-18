#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

// engine includes
#include "Memory\BlockAllocator.h"

// game includes
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
	void PrintMessage(const char* i_message);
	void PrintMessage();
	void PrintMessageMonsterName();	
	void PrintGameInformation();

	// functions to scan and validate user input
	void AcceptInput();
	void ValidateInput(const char* i_input);
	void ValidateNumber(const char* i_input);
	void ValidateName(const char* i_input);
	void ValidateMove(const char* i_input);

	// game logic
	void SaveNumMonsters(int i_num_monsters);
	void CreateMonster(const char* i_input_name = nullptr);
	void DestroyMonster(int i_at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* i_name);

	void CreatePlayer(const char* i_name);

	inline GameStates GetState() const												{ return game_state_; }
	static inline engine::memory::BlockAllocator* GetAllocator()					{ return MonsterChase::game_allocator_; }

	// game constants
	static const size_t									MEMORY_SIZE = 1024 * 5;
	static const int									MAX_INPUT_SIZE = 256;
	static const int									MAX_ROWS = 50;
	static const int									MAX_COLS = 50;
	static const int									START_ASCII = 97;
	static const int									MAX_ASCII = 25;
	static const int									MAX_MONSTERS = 10;
	static const int									MAX_MONSTER_TTL = 10;
	static const int									MAX_NAME_LENGTH = 10;

private:
	// disable default copy constructor
	MonsterChase(const MonsterChase& i_copy);
	// disable default assignment operator
	MonsterChase& operator=(const MonsterChase& i_monster);

private:
	// reference to an instance of an allocator specifically for game objects
	static engine::memory::BlockAllocator*				game_allocator_;

	// field to maintain the current state of the game
	GameStates											game_state_;

	// game elements
	Player*												player_;
	Monster**											monsters_;

	// game counters
	int													initial_num_monsters_;
	int													num_monsters_;
	int													ascii_index_;
}; // class MonsterChase

#endif // MONSTER_CHASE_H_