#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\IdentityComponent.h"
#include "Allocator\AllocatorUtil.h"
#include "Assert\Assert.h"

// game includes
#include "Game\PlayerController.h"

class Player
{
public:
	Player() : controller_(new PlayerController()),
		identity_(new engine::IdentityComponent())
	{}

	~Player()
	{
		SAFE_DELETE(controller_);
		SAFE_DELETE(identity_);
	}

	void Update();
	bool HandleUserInput(char input);
	void Print();

	// accessors and mutators
	inline engine::InterfaceGameObjectController* GetController() const								{ return controller_; }
	inline void SetController(engine::InterfaceGameObjectController* controller)					{ ASSERT(controller); SAFE_DELETE(controller_); controller_ = controller; }

	inline engine::IdentityComponent* GetIdentity() const											{ return identity_; }
	inline void SetIdentity(engine::IdentityComponent* identity)									{ ASSERT(identity); SAFE_DELETE(identity_); identity_ = identity; }

private:
	engine::InterfaceGameObjectController* controller_;
	engine::IdentityComponent* identity_;
}; // class Player

#endif // PLAYER_H_
