#include "Player.h"
#include "MonsterChase.h"

// include libraries
#include <stdio.h>

Player::Player()
{
	memset(name_, 0, sizeof(name_));
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
		new_column = position_.x() - 1;
		position_.x((new_column < 0) ? 0 : new_column);
		break;
	case MoveDirectionRight:
		new_column = position_.x() + 1;
		position_.x((new_column >= MonsterChase::MAX_COLS) ? MonsterChase::MAX_COLS - 1 : new_column);
		break;
	case MoveDirectionUp:
		new_row = position_.y() - 1;
		position_.y((new_row < 0) ? 0 : new_row);
		break;
	case MoveDirectionDown:
		new_row = position_.y() + 1;
		position_.y((new_row >= MonsterChase::MAX_ROWS) ? MonsterChase::MAX_ROWS - 1 : new_row);
		break;
	}
}

void Player::Print()
{
	printf("Player %s is at [%f, %f]\n", name_, position_.x(), position_.y());
}
