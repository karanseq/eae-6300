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
	~Monster();

	void Update();

	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const								{ return controller_; }
	inline void SetController(engine::gameobject::InterfaceGameObjectController* controller)					{ ASSERT(controller); SAFE_DELETE(controller_); controller_ = controller; }

	inline engine::gameobject::IdentityComponent* GetIdentity() const											{ return identity_; }
	inline void SetIdentity(engine::gameobject::IdentityComponent* identity)									{ ASSERT(identity); SAFE_DELETE(identity_); identity_ = identity; }

	inline uint8_t GetTimeToLive() const															{ return time_to_live_; }
	inline void SetTimeToLive(uint8_t time_to_live)													{ time_to_live_ = time_to_live; }

private:
	// disable default copy constructor
	Monster(const Monster& copy);
	// disable default assignment operator
	Monster& operator=(const Monster& monster);

private:
	engine::gameobject::InterfaceGameObjectController*			controller_;
	engine::gameobject::IdentityComponent*						identity_;
	uint8_t														time_to_live_;
}; // class Monster

#endif // MONSTER_H_