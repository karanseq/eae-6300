#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\IdentityComponent.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

// game includes
#include "Game\PlayerController.h"

class Player
{
public:
	Player(const char* name);
	~Player();

	void Update();
	bool HandleUserInput(char input);
	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const								{ return controller_; }
	inline void SetController(engine::gameobject::InterfaceGameObjectController* controller)					{ ASSERT(controller); SAFE_DELETE(controller_); controller_ = controller; }

	inline engine::gameobject::IdentityComponent* GetIdentity() const											{ return identity_; }
	inline void SetIdentity(engine::gameobject::IdentityComponent* identity)									{ ASSERT(identity); SAFE_DELETE(identity_); identity_ = identity; }

private:
	// disable default copy constructor
	Player(const Player& copy);
	// disable default assignment operator
	Player& operator=(const Player& monster);

private:
	engine::gameobject::InterfaceGameObjectController*					controller_;
	engine::gameobject::IdentityComponent*								identity_;
}; // class Player

#endif // PLAYER_H_
