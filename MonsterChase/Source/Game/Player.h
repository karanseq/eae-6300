#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\IdentityComponent.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\PlayerController.h"

class Player
{
public:
	Player();
	~Player();

	void Update();
	bool HandleUserInput(char i_input);
	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const								{ return controller_; }
	inline void SetController(engine::gameobject::InterfaceGameObjectController* i_controller)					{ ASSERT(i_controller); SAFE_DELETE(controller_); controller_ = i_controller; }

	inline engine::gameobject::IdentityComponent* GetIdentity() const											{ return identity_; }
	inline void SetIdentity(engine::gameobject::IdentityComponent* i_identity)									{ ASSERT(i_identity); SAFE_DELETE(identity_); identity_ = i_identity; }

private:
	// disable default copy constructor
	Player(const Player& i_copy);
	// disable default assignment operator
	Player& operator=(const Player& i_player);

private:
	engine::gameobject::InterfaceGameObjectController*					controller_;
	engine::gameobject::IdentityComponent*								identity_;
}; // class Player

#endif // PLAYER_H_
