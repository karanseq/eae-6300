#include "Game\Player.h"

// library includes
#include <stdio.h>

void Player::Update()
{
	controller_->UpdateGameObject();
}

void Player::Move(MoveDirections move_direction)
{
	reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(move_direction);
}

void Player::Print()
{
	printf("Player %s is at [%f, %f]\n", identity_->GetName(), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}