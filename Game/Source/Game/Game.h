#ifndef MONSTER_CHASE_H_
#define MONSTER_CHASE_H_

// library includes
#include <stdint.h>
#include <vector>

// engine includes
#include "GameObject\Actor.h"
#include "Memory\SharedPointer.h"
#include "Input\KeyboardEvent.h"
#include "Time\InterfaceTickable.h"

// game includes
#include "Game\GameTypes.h"

namespace game {

// forward declarations
class Player;

// global life-cycle functions
bool StartUp();
bool Restart();
void Shutdown();

class Game : public engine::time::InterfaceTickable
{
private:
	Game();
	~Game();
	static Game* instance_;

	// disable copy constructor & copy assignment operator
	Game(const Game& i_copy) = delete;
	Game& operator=(const Game& i_copy) = delete;

public:
	static Game* Create();
	static inline Game* GetInstance()										{ return instance_; }
	static void Destroy();

	// game initialization
	bool Init();
	bool LoadGameData();

	// the main game loop
	// called every tick by the engine
	virtual void Update(float dt) override;

	// gameplay
	void OnKeyPressed(unsigned int i_key_id);
	void CreatePlayer();

	inline GameStates GetState() const												{ return game_state_; }

	// game constants
	static const uint16_t															SCREEN_WIDTH = 1280;
	static const uint16_t															SCREEN_HEIGHT = 800;

private:
	// field to maintain the current state of the game
	GameStates																		game_state_;

	// game elements
	Player*																			player_;
	std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>			monsters_;
	engine::memory::SharedPointer<engine::input::KeyboardEvent>						keyboard_event_;

}; // class MonsterChase

} // namespace monsterchase

#endif // MONSTER_CHASE_H_