#include "Game\SmartMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

SmartMonsterController::SmartMonsterController() : game_object_(new (MonsterChase::GetAllocator()) engine::GameObject()),
	target_(nullptr)
{}

SmartMonsterController::~SmartMonsterController()
{
	SAFE_DELETE(game_object_);
}

void SmartMonsterController::UpdateGameObject()
{
	ASSERT(target_);

	const engine::Vec3D& my_position = game_object_->GetPosition();
	const engine::Vec3D& target_position = target_->GetPosition();

	if (my_position.y() < target_position.y())
	{
		game_object_->SetPosition(game_object_->GetPosition() + engine::Vec3D::UNIT_Y);
	}
	else if (my_position.y() > target_position.y())
	{
		game_object_->SetPosition(game_object_->GetPosition() - engine::Vec3D::UNIT_Y);
	}
	else if (my_position.x() < target_position.x())
	{
		game_object_->SetPosition(game_object_->GetPosition() + engine::Vec3D::UNIT_X);
	}
	else if (my_position.x() > target_position.x())
	{
		game_object_->SetPosition(game_object_->GetPosition() - engine::Vec3D::UNIT_X);
	}
}