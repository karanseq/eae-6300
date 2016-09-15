#include "Player.h"

// include libraries
#include <stdio.h>

namespace engine
{

int Player::max_rows_ = 0;
int Player::max_cols_ = 0;

Player::Player()
{
	memset(name_, 0, sizeof(name_));
}

Player::Player(const Vec2D& position, const char* name)
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
		position_.x((new_column >= Player::max_cols_) ? Player::max_cols_ - 1 : new_column);
		break;
	case MoveDirectionUp:
		new_row = position_.y() - 1;
		position_.y((new_row < 0) ? 0 : new_row);
		break;
	case MoveDirectionDown:
		new_row = position_.y() + 1;
		position_.y((new_row >= Player::max_rows_) ? Player::max_rows_ - 1 : new_row);
		break;
	}
}

void Player::Print()
{
	printf("Player %s is at [%f, %f]\n", name_, position_.x(), position_.y());
}

} // namespace engine
