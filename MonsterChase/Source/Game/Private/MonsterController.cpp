#include "Game\MonsterController.h"

#include <stdlib.h>
#include <string.h>

MonsterController::~MonsterController()
{
	if (name_)
	{
		delete name_;
		name_ = nullptr;
	}
}

void MonsterController::UpdateGameObject()
{
	bool has_moved = false;
	if (target_)
	{
		const engine::Vec2D& my_position = monster_->GetPosition();
		const engine::Vec2D& target_position = target_->GetPosition();

		if (my_position.y() < target_position.y())
		{
			Move(engine::MovementController::MoveDirection2D::MoveDirectionUp);
			has_moved = true;
		}
		else if (my_position.y() > target_position.y())
		{
			Move(engine::MovementController::MoveDirection2D::MoveDirectionDown);
			has_moved = true;
		}
		else if (my_position.x() < target_position.x())
		{
			Move(engine::MovementController::MoveDirection2D::MoveDirectionRight);
			has_moved = true;
		}
		else if (my_position.x() > target_position.x())
		{
			Move(engine::MovementController::MoveDirection2D::MoveDirectionLeft);
			has_moved = true;
		}
	}

	if (!has_moved)
	{
		int direction = rand() % 4;
		Move(static_cast<engine::MovementController::MoveDirection2D>(direction));
	}

	--time_to_live_;
}

void MonsterController::Move(engine::MovementController::MoveDirection2D direction)
{
	ASSERT(monster_ != nullptr);

	switch (direction)
	{
	case engine::MovementController::MoveDirection2D::MoveDirectionLeft:
		monster_->SetPosition(monster_->GetPosition() - engine::Vec2D::UNIT_X);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionRight:
		monster_->SetPosition(monster_->GetPosition() + engine::Vec2D::UNIT_X);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionUp:
		monster_->SetPosition(monster_->GetPosition() + engine::Vec2D::UNIT_Y);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionDown:
		monster_->SetPosition(monster_->GetPosition() - engine::Vec2D::UNIT_Y);
		break;
	}
}

void MonsterController::SetName(const char* name)
{
	if (name_)
	{
		delete name_;
		name_ = nullptr;
	}

	name_ = _strdup(name);
}
