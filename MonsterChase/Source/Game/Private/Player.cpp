#include "Game\Player.h"

// library includes
#include <stdio.h>

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

Player::Player(const char* name) : controller_(new (MonsterChase::GetAllocator()) PlayerController()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(0, 0, name))
{}

Player::~Player()
{
	SAFE_DELETE(controller_);
	SAFE_DELETE(identity_);
}

Player::Player(const Player& copy) : controller_(copy.controller_->Clone()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(copy.identity_->GetID(), copy.identity_->GetTag(), copy.identity_->GetName()))
{}

Player::Player(Player&& copy) : controller_(copy.controller_),
	identity_(copy.identity_)
{
	copy.controller_ = nullptr;
	copy.identity_ = nullptr;
}

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