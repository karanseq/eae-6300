#ifndef SMART_MONSTER_CONTROLLER_H_
#define SMART_MONSTER_CONTROLLER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class SmartMonsterController : public engine::InterfaceGameObjectController
{
public:
	SmartMonsterController() : game_object_(new engine::GameObject()),
		target_(nullptr)
	{}

	virtual ~SmartMonsterController()
	{
		SAFE_DELETE(game_object_);
	}

	/* Implement InterfaceGameObjectController */
	inline void SetGameObject(engine::GameObject* game_object) override				{ ASSERT(game_object); SAFE_DELETE(game_object_); game_object_ = game_object; }
	inline engine::GameObject* GetGameObject() override								{ return game_object_; }

	void UpdateGameObject() override;

	inline void SetTarget(engine::GameObject* target)								{ ASSERT(target); SAFE_DELETE(target_); target_ = target; }
	inline engine::GameObject* GetTarget()											{ return target_; }

private:
	// disable default copy constructor
	SmartMonsterController(const SmartMonsterController& copy);
	// disable default assignment operator
	SmartMonsterController& operator=(const SmartMonsterController& monster);

private:
	engine::GameObject* game_object_;
	engine::GameObject* target_;
}; // class SmartMonsterController

#endif // SMART_MONSTER_CONTROLLER_H_
