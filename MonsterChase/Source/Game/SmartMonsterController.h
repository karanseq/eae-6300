#ifndef SMART_MONSTER_CONTROLLER_H_
#define SMART_MONSTER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

class SmartMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SmartMonsterController();
	virtual ~SmartMonsterController();

	/* Implement InterfaceGameObjectController */
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override				{ ASSERT(i_game_object); SAFE_DELETE(game_object_); game_object_ = i_game_object; }
	inline engine::gameobject::GameObject* GetGameObject() override									{ return game_object_; }

	void UpdateGameObject() override;

	inline void SetTarget(engine::gameobject::GameObject* i_target)									{ ASSERT(i_target); SAFE_DELETE(target_); target_ = i_target; }
	inline engine::gameobject::GameObject* GetTarget()												{ return target_; }

private:
	// disable default copy constructor
	SmartMonsterController(const SmartMonsterController& i_copy);
	// disable default assignment operator
	SmartMonsterController& operator=(const SmartMonsterController& i_controller);

private:
	engine::gameobject::GameObject* game_object_;
	engine::gameobject::GameObject* target_;
}; // class SmartMonsterController

#endif // SMART_MONSTER_CONTROLLER_H_
