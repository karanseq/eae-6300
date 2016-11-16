#ifndef MONSTER_H_
#define MONSTER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\IdentityComponent.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

// game includes
#include "Game\GameTypes.h"
#include "Game\SmartMonsterController.h"
#include "Game\SillyMonsterController.h"

class Monster
{
public:
	Monster(MonsterControllers controller_type = MonsterControllers::kNoMonsterController);

	~Monster()
	{
		SAFE_DELETE(controller_);
		SAFE_DELETE(identity_);
	}

	void Update();

	void Print();

	// accessors and mutators
	inline engine::InterfaceGameObjectController* GetController() const								{ return controller_; }
	inline void SetController(engine::InterfaceGameObjectController* controller)					{ ASSERT(controller); SAFE_DELETE(controller_); controller_ = controller; }

	inline engine::IdentityComponent* GetIdentity() const											{ return identity_; }
	inline void SetIdentity(engine::IdentityComponent* identity)									{ ASSERT(identity); SAFE_DELETE(identity_); identity_ = identity; }

	inline uint8_t GetTimeToLive() const															{ return time_to_live_; }
	inline void SetTimeToLive(uint8_t time_to_live)													{ time_to_live_ = time_to_live; }

private:
	engine::InterfaceGameObjectController* controller_;
	engine::IdentityComponent* identity_;
	uint8_t time_to_live_;
}; // class Monster

#endif // MONSTER_H_