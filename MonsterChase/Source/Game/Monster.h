#ifndef MONSTER_H_
#define MONSTER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\IdentityComponent.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\GameTypes.h"
#include "Game\SillyMonsterController.h"
#include "Game\SmartMonsterController.h"

class Monster
{
public:
	Monster(MonsterControllers controller_type, const char* name);
	~Monster();

	// copy constructor
	Monster(const Monster& copy);
	// move constructor
	Monster(Monster&& copy);

	// copy assignment operator
	inline Monster& operator=(const Monster& monster);
	// move assignment operator
	inline Monster& operator=(Monster&& monster);

	void Update();

	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const;
	inline void SetController(engine::gameobject::InterfaceGameObjectController* controller);

	inline engine::gameobject::IdentityComponent* GetIdentity() const;
	inline void SetIdentity(engine::gameobject::IdentityComponent* identity);

	inline uint8_t GetTimeToLive() const;
	inline void SetTimeToLive(uint8_t time_to_live);

private:
	engine::gameobject::InterfaceGameObjectController*			controller_;
	engine::gameobject::IdentityComponent*						identity_;
	uint8_t														time_to_live_;
}; // class Monster

#include "Game\Monster-inl.h"

#endif // MONSTER_H_