#include "Game\SillyMonsterController.h"
#include "Game\GameTypes.h"
#include "Game\GameUtils.h"

void SillyMonsterController::UpdateGameObject()
{
	MoveDirections direction = GameUtils::GetRandomDirection();
	switch (direction)
	{
	case MoveDirections::kMoveDirectionLeft:
		game_object_->SetPosition(game_object_->GetPosition() - engine::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionRight:
		game_object_->SetPosition(game_object_->GetPosition() + engine::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionUp:
		game_object_->SetPosition(game_object_->GetPosition() + engine::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionDown:
		game_object_->SetPosition(game_object_->GetPosition() - engine::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionNone:
	default:
		break;
	}
}
