#include "Game\SillyMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\GameTypes.h"
#include "Game\GameUtils.h"
#include "Game\MonsterChase.h"

SillyMonsterController::SillyMonsterController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject())
{}

SillyMonsterController::~SillyMonsterController()
{
	SAFE_DELETE(game_object_);
}

void SillyMonsterController::UpdateGameObject()
{
	MoveDirections direction = GameUtils::GetRandomDirection();
	switch (direction)
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
