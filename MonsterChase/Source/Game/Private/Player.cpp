#include "Game\Player.h"

// library includes
#include <stdio.h>

void Player::Update()
{
	controller_->UpdateGameObject();
}

bool Player::HandleUserInput(char input)
{
	if (input == 'a' || input == 'A')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionLeft);
		return true;
	}
	else if (input == 'd' || input == 'D')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionRight);
		return true;
	}
	else if (input == 'w' || input == 'W')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionUp);
		return true;
	}
	else if (input == 's' || input == 'S')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionDown);
		return true;
	}
	else
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionNone);
		return false;
	}
}

void Player::Print()
{
	printf("Player %s is at [%f, %f]\n", identity_->GetName(), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}