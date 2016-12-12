#ifndef SILLY_MONSTER_CONTROLLER_H_
#define SILLY_MONSTER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

class SillyMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SillyMonsterController();
	SillyMonsterController(engine::gameobject::GameObject* game_object);
	virtual ~SillyMonsterController();

	// copy constructor
	SillyMonsterController(const SillyMonsterController& copy);
	// move contructor
	SillyMonsterController(SillyMonsterController&& copy);

	// copy assignment operator
	inline SillyMonsterController& operator=(const SillyMonsterController& controller);
	// move assignment operator
	inline SillyMonsterController& operator=(SillyMonsterController&& controller);

	/* Implement InterfaceGameObjectController */
	SillyMonsterController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject() override;
	inline void SetGameObject(engine::gameobject::GameObject* game_object) override;

	void UpdateGameObject() override;

private:
	engine::gameobject::GameObject* game_object_;
}; // class SillyMonsterController

#include "SillyMonsterController-inl.h"

#endif // SILLY_MONSTER_CONTROLLER_H_

