#ifndef SILLY_MONSTER_CONTROLLER_H_
#define SILLY_MONSTER_CONTROLLER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class SillyMonsterController : public engine::InterfaceGameObjectController
{
public:
	SillyMonsterController() : game_object_(new engine::GameObject())
	{}

	virtual ~SillyMonsterController()
	{
		SAFE_DELETE(game_object_);
	}

	/* Implement InterfaceGameObjectController */
	inline void SetGameObject(engine::GameObject* game_object) override				{ ASSERT(game_object); SAFE_DELETE(game_object_); game_object_ = game_object; }
	inline engine::GameObject* GetGameObject() override								{ return game_object_; }

	void UpdateGameObject() override;

private:
	// disable default copy constructor
	SillyMonsterController(const SillyMonsterController& copy);
	// disable default assignment operator
	SillyMonsterController& operator=(const SillyMonsterController& monster);

private:
	engine::GameObject* game_object_;
}; // class SillyMonsterController

#endif // SILLY_MONSTER_CONTROLLER_H_

