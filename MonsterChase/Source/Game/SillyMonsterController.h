#ifndef SILLY_MONSTER_CONTROLLER_H_
#define SILLY_MONSTER_CONTROLLER_H_

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class SillyMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SillyMonsterController();
	virtual ~SillyMonsterController();

	/* Implement InterfaceGameObjectController */
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override				{ ASSERT(i_game_object); SAFE_DELETE(game_object_); game_object_ = i_game_object; }
	inline engine::gameobject::GameObject* GetGameObject() override									{ return game_object_; }

	void UpdateGameObject() override;

private:
	// disable default copy constructor
	SillyMonsterController(const SillyMonsterController& i_copy);
	// disable default assignment operator
	SillyMonsterController& operator=(const SillyMonsterController& i_controller);

private:
	engine::gameobject::GameObject* game_object_;
}; // class SillyMonsterController

#endif // SILLY_MONSTER_CONTROLLER_H_

