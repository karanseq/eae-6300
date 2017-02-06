#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

// library includes
#include <stdint.h>

// engine includes
#include "Time\InterfaceTickable.h"

// game includes
#include "Game\GameTypes.h"

namespace monsterchase {

// forward declarations
class Player;

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

	// disable copy constructor & copy assignment operator
	MonsterChase(const MonsterChase& i_copy) = delete;
	MonsterChase& operator=(const MonsterChase& i_copy) = delete;

public:
	static MonsterChase* Create();
	static inline MonsterChase* GetInstance()										{ return instance_; }
	static void Destroy();

	// game initialization
	bool Init();
	// the main game loop
	// called every tick by the engine
	virtual void Update(float dt) override;

	// I/O functions
	void CheckInput();
	bool LoadGameData();

	// game logic
	void CreatePlayer();

	inline GameStates GetState() const												{ return game_state_; }

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
	// field to maintain the current state of the game
	GameStates											game_state_;

	// game elements
	Player*												player_;

}; // class MonsterChase

} // namespace monsterchase

#endif // MONSTER_CHASE_H_