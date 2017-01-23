#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\GameTypes.h"

// forward declarations
namespace engine {
	namespace physics {
		class PhysicsObject;
	}
}

namespace monsterchase {

class PlayerController : public engine::gameobject::InterfaceGameObjectController
{
public:
	PlayerController();
	PlayerController(engine::gameobject::GameObject* i_game_object, engine::physics::PhysicsObject* i_physics_object);
	virtual ~PlayerController();

	// copy constructor
	PlayerController(const PlayerController& i_copy);
	// move constructor
	PlayerController(PlayerController&& i_copy);

	// copy assignment operator
	inline PlayerController& operator=(const PlayerController& i_controller);
	// move assignment operator
	inline PlayerController& operator=(PlayerController&& i_controller);

	// Implement InterfaceGameObjectController
	PlayerController* Clone() const override;
	
	inline engine::gameobject::GameObject* GetGameObject() const override;
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override;

	void UpdateGameObject() override;

	// functions
	void Move(MoveDirections i_move_direction);

	// accessors & mutators
	inline engine::physics::PhysicsObject* GetPhysicsObject() const;
	inline void SetPhysicsObject(engine::physics::PhysicsObject* i_physics_object);

private:
	engine::gameobject::GameObject*							game_object_;
	engine::physics::PhysicsObject*							physics_object_;

}; // class PlayerController

} // namespace monsterchase

#include "PlayerController-inl.h"

#endif // PLAYER_CONTROLLER_H_
