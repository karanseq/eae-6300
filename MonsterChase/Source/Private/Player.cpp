#include "Player.h"
#include "MonsterChase.h"

// include libraries
#include <stdio.h>

Player::Player()
{
#ifdef BUILD_DEBUG
	memset(name_, 0, sizeof(name_));
#endif
}

Player::Player(const engine::Vec2D& position, const char* name)
{
	SetPosition(position);
	SetName(name);
}

Player::~Player()
{}

void Player::Move(MoveDirection move_direction)
{
	float new_row = 0, new_column = 0;

	switch (move_direction)
	{
	case MoveDirectionLeft:
		position_ -= engine::Vec2D::UNIT_X;
		break;
	case MoveDirectionRight:
		position_ += engine::Vec2D::UNIT_X;
		break;
	case MoveDirectionUp:
		position_ += engine::Vec2D::UNIT_Y;
		break;
	case MoveDirectionDown:
		position_ -= engine::Vec2D::UNIT_Y;
		break;
	}
}

void Player::Print() const
{
	printf("Player %s is at [%f, %f]\n", name_, position_.x(), position_.y());
}
