#ifndef SMART_MONSTER_CONTROLLER_H_
#define SMART_MONSTER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

namespace monsterchase {

class SmartMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SmartMonsterController();
	SmartMonsterController(engine::gameobject::GameObject* i_game_object);
	virtual ~SmartMonsterController();

	// copy constructor
	SmartMonsterController(const SmartMonsterController& i_copy);
	// move constructor
	SmartMonsterController(SmartMonsterController&& i_copy);

	// copy assignment operator
	inline SmartMonsterController& operator=(const SmartMonsterController& i_controller);
	// move assignment operator
	inline SmartMonsterController& operator=(SmartMonsterController&& i_controller);

	/* Implement InterfaceGameObjectController */
	SmartMonsterController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject() override;
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override;
	void UpdateGameObject() override;

	inline engine::gameobject::GameObject* GetTarget();
	inline void SetTarget(engine::gameobject::GameObject* i_target);

private:
	engine::gameobject::GameObject* game_object_;
	engine::gameobject::GameObject* target_;
}; // class SmartMonsterController

} // namespace monsterchase

#include "SmartMonsterController-inl.h"

#endif // SMART_MONSTER_CONTROLLER_H_
