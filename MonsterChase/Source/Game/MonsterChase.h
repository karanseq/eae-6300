#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

// library includes
#include <vector>

// engine includes
#include "Time\InterfaceTickable.h"

// game includes
#include "Game\GameTypes.h"

// forward declarations
namespace engine {
namespace memory {
	class BlockAllocator;
}
}

namespace monsterchase {
// forward declarations
class Player;
class Monster;

// TODO: Find a better place for this
void AcceptKey(unsigned int i_key_id, bool i_went_down);

bool StartUp();

void Shutdown();

class MonsterChase : public engine::time::InterfaceTickable
{
private:
	MonsterChase();
	~MonsterChase();
	static MonsterChase* instance_;

public:
	static MonsterChase* Create();
	static inline MonsterChase* GetInstance()										{ return instance_; }
	static void Destroy();

	// game initialization
	bool Init();
	// the main game loop
	// called every tick by the engine
	virtual void Update(float dt) override;
	// the rendering loop
	// called every tick from Update
	void Render();

	// I/O functions
	void PrintGameInformation();
	void CheckInput();
	bool LoadGameData();

	// game logic
	void CreateMonster(const char* i_input_name = nullptr);
	void DestroyMonster(uint8_t i_at_index);
	void UpdateMonsters();
	void GetNameForMonster(char* o_name);
	void CreatePlayer(const char* i_name);

	inline GameStates GetState() const												{ return game_state_; }
	static inline engine::memory::BlockAllocator* GetAllocator()					{ return MonsterChase::game_allocator_; }

	static bool											KEY_A_PRESSED;
	static bool											KEY_D_PRESSED;
	static bool											KEY_M_PRESSED;
	static bool											KEY_Q_PRESSED;
	static bool											KEY_S_PRESSED;
	static bool											KEY_W_PRESSED;

	// game constants
	static const size_t									MEMORY_SIZE = 1024 * 5;
	static const uint16_t								MAX_INPUT_SIZE = 256;
	static const uint8_t								MAX_ROWS = 30;
	static const uint8_t								MAX_COLS = 60;
	static const uint8_t								START_ASCII = 97;
	static const uint8_t								MAX_ASCII = 25;
	static const uint8_t								MAX_MONSTERS = 10;
	static const uint8_t								MAX_MONSTER_TTL = 25;
	static const uint8_t								MAX_NAME_LENGTH = 10;
	static const uint16_t								SCREEN_WIDTH = 1280;
	static const uint16_t								SCREEN_HEIGHT = 800;

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
	std::vector<Monster*>								monsters_;

	// game counters
	uint8_t												num_monsters_;
	uint8_t												ascii_index_;
}; // class MonsterChase

} // namespace monsterchase

#endif // MONSTER_CHASE_H_