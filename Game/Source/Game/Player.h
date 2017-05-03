#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "GameObject\Actor.h"
#include "Events\KeyboardEvent.h"
#include "Memory\SharedPointer.h"
#include "Time\InterfaceTickable.h"

namespace game {
	
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
	void Tick(float i_dt) override;

	// keyboard event handlers
	void OnKeyPressed(unsigned int i_key_id);
	void OnKeyReleased(unsigned int i_key_id);
    void SetKeys(const char i_left, const char i_right, const char i_up, const char i_down);

    // accessor
    inline const engine::memory::WeakPointer<engine::gameobject::Actor> GetActor() { return actor_; }

	// constants
	static const float																DEFAULT_MASS;
	static const float																DEFAULT_FORCE;

private:
    char                                                                            left_key_;
	bool																			is_left_pressed_;
    char                                                                            right_key_;
	bool																			is_right_pressed_;
    char                                                                            up_key_;
	bool																			is_up_pressed_;
    char                                                                            down_key_;
	bool																			is_down_pressed_;

	engine::memory::SharedPointer<engine::gameobject::Actor>						actor_;
	engine::memory::SharedPointer<engine::events::KeyboardEvent>					keyboard_event_;

}; // class Player

} // namespace game

#endif // PLAYER_H_
