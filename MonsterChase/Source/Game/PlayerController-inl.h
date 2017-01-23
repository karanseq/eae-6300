#include "PlayerController.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Physics\PhysicsObject.h"

// game includes
#include "MonsterChase.h"

namespace monsterchase {

inline PlayerController& PlayerController::operator=(const PlayerController& i_controller)
{
	if (this != &i_controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(*(i_controller.game_object_));

		SAFE_DELETE(physics_object_);
		physics_object_ = new (MonsterChase::GetAllocator()) engine::physics::PhysicsObject(*(i_controller.physics_object_));
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

inline engine::gameobject::GameObject* PlayerController::GetGameObject() const
{
	return game_object_;
}

inline void PlayerController::SetGameObject(engine::gameobject::GameObject* i_game_object)
{
	ASSERT(i_game_object);
	SAFE_DELETE(game_object_);
	game_object_ = i_game_object;
}

inline engine::physics::PhysicsObject* PlayerController::GetPhysicsObject() const
{
	return physics_object_;
}

inline void PlayerController::SetPhysicsObject(engine::physics::PhysicsObject* i_physics_object)
{
	ASSERT(i_physics_object);
	SAFE_DELETE(physics_object_);
	physics_object_ = i_physics_object;
}

} // namespace monsterchase