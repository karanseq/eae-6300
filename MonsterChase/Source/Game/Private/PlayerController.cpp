#include "Game\PlayerController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"
#include "Math\Vec3D.h"

// game includes
#include "Game\MonsterChase.h"

PlayerController::PlayerController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject()),
	move_direction_(MoveDirections::kMoveDirectionNone)
{}

PlayerController::~PlayerController()
{
	SAFE_DELETE(game_object_);
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
