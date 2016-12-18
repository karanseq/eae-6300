#include "Game\PlayerController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"
#include "Math\Vec3D.h"

// game includes
#include "Game\MonsterChase.h"

PlayerController::PlayerController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject()),
	move_direction_(MoveDirections::kMoveDirectionNone)
{}

PlayerController::PlayerController(engine::gameobject::GameObject* i_game_object) : game_object_(i_game_object),
	move_direction_(MoveDirections::kMoveDirectionNone)
{}

PlayerController::~PlayerController()
{
	SAFE_DELETE(game_object_);
}

PlayerController::PlayerController(const PlayerController& i_copy) : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(i_copy.game_object_->GetTransform())),
	move_direction_(i_copy.move_direction_)
{}

PlayerController::PlayerController(PlayerController&& i_copy) : game_object_(i_copy.game_object_),
	move_direction_(i_copy.move_direction_)
{
	i_copy.game_object_ = nullptr;
}

PlayerController* PlayerController::Clone() const
{
	// first clone the game object this controller maintains
	engine::gameobject::GameObject* game_object_clone = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(game_object_->GetTransform());
	// now create a new controller with the cloned game object
	PlayerController* controller = new (MonsterChase::GetAllocator()) PlayerController(game_object_clone);
	controller->move_direction_ = move_direction_;
	return controller;
}

void PlayerController::UpdateGameObject()
{
	switch (move_direction_)
	{
	case MoveDirections::kMoveDirectionLeft:
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionRight:
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionUp:
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionDown:
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionNone:
	default:
		break;
	}
}
