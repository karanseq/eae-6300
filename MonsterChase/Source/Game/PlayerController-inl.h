#include "PlayerController.h"

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "Physics\PhysicsObject.h"

namespace monsterchase {

inline PlayerController& PlayerController::operator=(const PlayerController& i_controller)
{
	if (this != &i_controller)
	{
		game_object_ = i_controller.game_object_;
		physics_object_ = i_controller.physics_object_;
	}
	return *this;
}

inline PlayerController& PlayerController::operator=(PlayerController&& i_controller)
{
	if (this != &i_controller)
	{
		std::swap(game_object_, i_controller.game_object_);
		std::swap(physics_object_, i_controller.physics_object_);
	}
	return *this;
}

inline engine::memory::SharedPointer<engine::gameobject::GameObject> PlayerController::GetGameObject() const
{
	return game_object_;
}

inline void PlayerController::SetGameObject(const engine::memory::SharedPointer<engine::gameobject::GameObject>& i_game_object)
{
	ASSERT(i_game_object);
	game_object_ = i_game_object;
}

inline engine::memory::SharedPointer<engine::physics::PhysicsObject> PlayerController::GetPhysicsObject() const
{
	return physics_object_;
}

inline void PlayerController::SetPhysicsObject(const engine::memory::SharedPointer<engine::physics::PhysicsObject>& i_physics_object)
{
	ASSERT(i_physics_object);
	physics_object_ = i_physics_object;
}

} // namespace monsterchase