#include "Game\PlayerController.h"

// engine includes
#include "Math\Vec3D.h"
#include "Common\HelperMacros.h"
#include "Memory\AllocatorOverrides.h"
#include "Physics\Physics.h"

// game includes
#include "Game\MonsterChase.h"

namespace monsterchase {

PlayerController::PlayerController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject()),
	physics_object_(new (MonsterChase::GetAllocator()) engine::physics::PhysicsObject(game_object_)),
	move_direction_(MoveDirections::kMoveDirectionNone)
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::PlayerController(engine::gameobject::GameObject* i_game_object, engine::physics::PhysicsObject* i_physics_object) : game_object_(i_game_object),
	physics_object_(i_physics_object),
	move_direction_(MoveDirections::kMoveDirectionNone)
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::~PlayerController()
{
	SAFE_DELETE(game_object_);
	if (physics_object_)
	{
		engine::physics::Physics::Get()->RemovePhysicsObject(physics_object_);
	}
	SAFE_DELETE(physics_object_);
}

PlayerController::PlayerController(const PlayerController& i_copy) : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(*(i_copy.game_object_))),
	physics_object_(new (MonsterChase::GetAllocator()) engine::physics::PhysicsObject(*(i_copy.physics_object_))),
	move_direction_(i_copy.move_direction_)
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::PlayerController(PlayerController&& i_copy) : game_object_(i_copy.game_object_),
	physics_object_(i_copy.physics_object_),
	move_direction_(i_copy.move_direction_)
{
	i_copy.game_object_ = nullptr;
	i_copy.physics_object_ = nullptr;
}

PlayerController* PlayerController::Clone() const
{
	// clone the objects this controller maintains
	engine::gameobject::GameObject* game_object = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(*game_object_);
	engine::physics::PhysicsObject* physics_object = new (MonsterChase::GetAllocator()) engine::physics::PhysicsObject(*physics_object_);

	// now create a new controller with the cloned game object
	PlayerController* controller = new (MonsterChase::GetAllocator()) PlayerController(game_object, physics_object);
	controller->move_direction_ = move_direction_;
	return controller;
}

void PlayerController::UpdateGameObject()
{
	static const float impulse = 0.1f;
	switch (move_direction_)
	{
	case MoveDirections::kMoveDirectionLeft:
		//game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_X);
		physics_object_->ApplyImpulse(engine::math::Vec3D(-impulse, 0.0f, 0.0f));
		break;

	case MoveDirections::kMoveDirectionRight:
		//game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_X);
		physics_object_->ApplyImpulse(engine::math::Vec3D(impulse, 0.0f, 0.0f));
		break;

	case MoveDirections::kMoveDirectionUp:
		//game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_Y);
		physics_object_->ApplyImpulse(engine::math::Vec3D(0.0f, impulse, 0.0f));
		break;

	case MoveDirections::kMoveDirectionDown:
		//game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_Y);
		physics_object_->ApplyImpulse(engine::math::Vec3D(0.0f, -impulse, 0.0f));
		break;

	case MoveDirections::kMoveDirectionNone:
	default:
		break;
	}
}

} // namespace monsterchase
