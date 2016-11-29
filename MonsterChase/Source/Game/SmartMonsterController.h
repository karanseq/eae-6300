#ifndef SMART_MONSTER_CONTROLLER_H_
#define SMART_MONSTER_CONTROLLER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class SmartMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SmartMonsterController();
	SmartMonsterController(engine::gameobject::GameObject* game_object);
	virtual ~SmartMonsterController();

	// copy constructor
	SmartMonsterController(const SmartMonsterController& copy);
	// move constructor
	SmartMonsterController(SmartMonsterController&& copy);

	// copy assignment operator
	inline SmartMonsterController& operator=(const SmartMonsterController& controller);
	// move assignment operator
	inline SmartMonsterController& operator=(SmartMonsterController&& controller);

	/* Implement InterfaceGameObjectController */
	SmartMonsterController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject() override;
	inline void SetGameObject(engine::gameobject::GameObject* game_object) override;
	void UpdateGameObject() override;

	inline engine::gameobject::GameObject* GetTarget();
	inline void SetTarget(engine::gameobject::GameObject* target);

private:
	engine::gameobject::GameObject* game_object_;
	engine::gameobject::GameObject* target_;
}; // class SmartMonsterController

#include "SmartMonsterController-inl.h"

#endif // SMART_MONSTER_CONTROLLER_H_
