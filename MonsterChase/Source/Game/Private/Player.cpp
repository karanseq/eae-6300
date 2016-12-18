#include "Game\Player.h"

// library includes
#include <stdio.h>

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

Player::Player() : controller_(new (MonsterChase::GetAllocator()) PlayerController()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent())
{}

Player::~Player()
{
	SAFE_DELETE(controller_);
	SAFE_DELETE(identity_);
}

void Player::Update()
{
	controller_->UpdateGameObject();
}

bool Player::HandleUserInput(char i_input)
{
	if (i_input == 'a' || i_input == 'A')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionLeft);
		return true;
	}
	else if (i_input == 'd' || i_input == 'D')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionRight);
		return true;
	}
	else if (i_input == 'w' || i_input == 'W')
	{
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionUp);
		return true;
	}
	else if (i_input == 's' || i_input == 'S')
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