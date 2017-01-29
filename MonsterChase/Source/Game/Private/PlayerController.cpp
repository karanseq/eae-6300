#include "Game\PlayerController.h"

// engine includes
#include "Assert\Assert.h"
#include "Math\Vec3D.h"
#include "Physics\Physics.h"

// game includes
#include "Game\MonsterChase.h"

namespace monsterchase {

PlayerController::PlayerController() : game_object_(engine::gameobject::GameObject::Create()),
	physics_object_(engine::physics::PhysicsObject::Create(game_object_))
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::PlayerController(engine::gameobject::GameObject* i_game_object, engine::physics::PhysicsObject* i_physics_object) : game_object_(i_game_object),
	physics_object_(i_physics_object)
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::~PlayerController()
{
	if (physics_object_)
	{
		engine::physics::Physics::Get()->RemovePhysicsObject(physics_object_);
	}
}

PlayerController::PlayerController(const PlayerController& i_copy) : game_object_(new engine::gameobject::GameObject(*(i_copy.game_object_))),
	physics_object_(new engine::physics::PhysicsObject(*(i_copy.physics_object_)))
{
	// validate state
	ASSERT(game_object_);
	ASSERT(physics_object_);

	engine::physics::Physics::Get()->AddPhysicsObject(physics_object_);
}

PlayerController::PlayerController(PlayerController&& i_copy) : game_object_(i_copy.game_object_),
	physics_object_(i_copy.physics_object_)
{
	i_copy.game_object_ = nullptr;
	i_copy.physics_object_ = nullptr;
}

PlayerController* PlayerController::Clone() const
{
	// clone the objects this controller maintains
	engine::gameobject::GameObject* game_object = new engine::gameobject::GameObject(*game_object_);
	engine::physics::PhysicsObject* physics_object = new engine::physics::PhysicsObject(*physics_object_);

	// now create a new controller with the cloned game object
	PlayerController* controller = new PlayerController(game_object, physics_object);
	return controller;
}

void PlayerController::UpdateGameObject()
{
	// wrap around the screen
	engine::math::Vec3D position = game_object_->GetPosition();
	position.x((position.x() < -MonsterChase::SCREEN_WIDTH / 2) ? MonsterChase::SCREEN_WIDTH / 2 : (position.x() > MonsterChase::SCREEN_WIDTH / 2 ? -MonsterChase::SCREEN_WIDTH / 2 : position.x()));
	position.y((position.y() < -MonsterChase::SCREEN_HEIGHT / 2) ? MonsterChase::SCREEN_HEIGHT / 2 : (position.y() > MonsterChase::SCREEN_HEIGHT / 2 ? -MonsterChase::SCREEN_HEIGHT / 2 : position.y()));
	game_object_->SetPosition(position);
}

void PlayerController::Move(MoveDirections i_move_direction)
{
	static const float impulse = 2.5f;
	switch (i_move_direction)
	{
	case MoveDirections::kMoveDirectionLeft:
		physics_object_->ApplyImpulse(engine::math::Vec3D(-impulse, 0.0f, 0.0f));
		break;

	case MoveDirections::kMoveDirectionRight:
		physics_object_->ApplyImpulse(engine::math::Vec3D(impulse, 0.0f, 0.0f));
		break;

	case MoveDirections::kMoveDirectionUp:
		physics_object_->ApplyImpulse(engine::math::Vec3D(0.0f, impulse, 0.0f));
		break;

	case MoveDirections::kMoveDirectionDown:
		physics_object_->ApplyImpulse(engine::math::Vec3D(0.0f, -impulse, 0.0f));
		break;

	case MoveDirections::kMoveDirectionNone:
	default:
		break;
	}
}

} // namespace monsterchase
