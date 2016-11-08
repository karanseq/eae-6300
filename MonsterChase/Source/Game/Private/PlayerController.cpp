#include "Game\PlayerController.h"

#include <string.h>

PlayerController::~PlayerController()
{
	if (name_)
	{
		delete name_;
		name_ = nullptr;
	}
}

void PlayerController::UpdateGameObject()
{ }

void PlayerController::Move(engine::MovementController::MoveDirection2D direction)
{
	ASSERT(player_ != nullptr);

	switch (direction)
	{
	case engine::MovementController::MoveDirection2D::MoveDirectionLeft:
		player_->SetPosition(player_->GetPosition() - engine::Vec2D::UNIT_X);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionRight:
		player_->SetPosition(player_->GetPosition() + engine::Vec2D::UNIT_X);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionUp:
		player_->SetPosition(player_->GetPosition() + engine::Vec2D::UNIT_Y);
		break;
	case engine::MovementController::MoveDirection2D::MoveDirectionDown:
		player_->SetPosition(player_->GetPosition() - engine::Vec2D::UNIT_Y);
		break;
	}
}

void PlayerController::SetName(const char* name)
{
	if (name_)
	{
		delete name_;
		name_ = nullptr;
	}

	name_ = _strdup(name);
}
