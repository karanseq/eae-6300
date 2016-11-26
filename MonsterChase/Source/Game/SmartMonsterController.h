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
	virtual ~SmartMonsterController();

	/* Implement InterfaceGameObjectController */
	inline void SetGameObject(engine::gameobject::GameObject* game_object) override				{ ASSERT(game_object); SAFE_DELETE(game_object_); game_object_ = game_object; }
	inline engine::gameobject::GameObject* GetGameObject() override								{ return game_object_; }

	void UpdateGameObject() override;

	inline void SetTarget(engine::gameobject::GameObject* target)								{ ASSERT(target); SAFE_DELETE(target_); target_ = target; }
	inline engine::gameobject::GameObject* GetTarget()											{ return target_; }

private:
	// disable default copy constructor
	SmartMonsterController(const SmartMonsterController& copy);
	// disable default assignment operator
	SmartMonsterController& operator=(const SmartMonsterController& monster);

private:
	engine::gameobject::GameObject* game_object_;
	engine::gameobject::GameObject* target_;
}; // class SmartMonsterController

#endif // SMART_MONSTER_CONTROLLER_H_
