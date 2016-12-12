#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

// library includes
#include <vector>

// game includes
#include "Game\GameTypes.h"

// forward declarations
namespace engine {
namespace memory {
	class BlockAllocator;
}
}
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
	void SaveNumMonsters(uint8_t num_monsters);
	void CreateMonster(const char* input_name = nullptr);
	void DestroyMonster(uint8_t at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* name);

	void CreatePlayer(const char* name);

	inline GameStates GetState() const												{ return game_state_; }
	static inline engine::memory::BlockAllocator* GetAllocator()					{ return MonsterChase::game_allocator_; }

	// game constants
	static const size_t									MEMORY_SIZE = 1024 * 5;
	static const uint16_t								MAX_INPUT_SIZE = 256;
	static const uint8_t								MAX_ROWS = 50;
	static const uint8_t								MAX_COLS = 50;
	static const uint8_t								START_ASCII = 97;
	static const uint8_t								MAX_ASCII = 25;
	static const uint8_t								MAX_MONSTERS = 10;
	static const uint8_t								MAX_MONSTER_TTL = 10;
	static const uint8_t								MAX_NAME_LENGTH = 10;

private:
	// disable default copy constructor
	MonsterChase(const MonsterChase& copy);
	// disable default assignment operator
	MonsterChase& operator=(const MonsterChase& monster);

private:
	// reference to an instance of an allocator specifically for game objects
	static engine::memory::BlockAllocator*				game_allocator_;

	// field to maintain the current state of the game
	GameStates											game_state_;

	// game elements
	Player*												player_;
	std::vector<Monster*>								monsters_;

	// game counters
	uint8_t												initial_num_monsters_;
	uint8_t												num_monsters_;
	uint8_t												ascii_index_;
}; // class MonsterChase

#endif // MONSTER_CHASE_H_