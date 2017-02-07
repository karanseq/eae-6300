#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "GameObject\Actor.h"
#include "Input\KeyboardEvent.h"
#include "Memory\SharedPointer.h"
#include "Time\InterfaceTickable.h"

namespace monsterchase {
	
class Player : public engine::time::InterfaceTickable
{
public:
	Player();
	~Player();

	// disable copy & move constructors & assignment operators
	Player(const Player& i_copy) = delete;
	Player& operator=(const Player& i_copy) = delete;
	Player(Player&& i_copy) = delete;
	Player& operator=(Player&& i_copy) = delete;

	// implement interface tickable
	void Update(float i_dt) override;

	// keyboard event handlers
	void OnKeyPressed(unsigned int i_key_id);
	void OnKeyReleased(unsigned int i_key_id);

	// constants
	static const float																DEFAULT_MASS;
	static const float																DEFAULT_FORCE;

private:
	bool																			is_left_pressed_;
	bool																			is_right_pressed_;
	bool																			is_up_pressed_;
	bool																			is_down_pressed_;

	engine::memory::SharedPointer<engine::gameobject::Actor>						actor_;
	engine::memory::SharedPointer<engine::input::KeyboardEvent>						keyboard_event_;

}; // class Player

} // namespace monsterchase

#endif // PLAYER_H_
