#ifndef SILLY_MONSTER_CONTROLLER_H_
#define SILLY_MONSTER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

namespace monsterchase {

class SillyMonsterController : public engine::gameobject::InterfaceGameObjectController
{
public:
	SillyMonsterController();
	SillyMonsterController(engine::gameobject::GameObject* i_game_object);
	virtual ~SillyMonsterController();

	// copy constructor
	SillyMonsterController(const SillyMonsterController& i_copy);
	// move contructor
	SillyMonsterController(SillyMonsterController&& i_copy);

	// copy assignment operator
	inline SillyMonsterController& operator=(const SillyMonsterController& i_controller);
	// move assignment operator
	inline SillyMonsterController& operator=(SillyMonsterController&& i_controller);

	/* Implement InterfaceGameObjectController */
	SillyMonsterController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject() const override;
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override;

	void UpdateGameObject() override;

private:
	engine::gameobject::GameObject* game_object_;
}; // class SillyMonsterController

} // namespace monsterchase

#include "SillyMonsterController-inl.h"

#endif // SILLY_MONSTER_CONTROLLER_H_