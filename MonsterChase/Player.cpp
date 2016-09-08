#include "Player.h"
#include "MonsterChase.h"

// include libraries
#include <stdio.h>

Player::Player()
{
	row_ = 0;
	column_ = 0;
	memset(name_, 0, sizeof(name_));
}

Player::Player(int row, int column, const char* name)
{
	SetRow(row);
	SetColumn(column);
	SetName(name);
}

Player::~Player()
{}

void Player::Move(MoveDirection move_direction)
{
	int new_row = 0, new_column = 0;

	switch (move_direction)
	{
	case MoveDirectionLeft:
		new_column = column_ - 1;
		column_ = (new_column < 0) ? 0 : new_column;
		break;
	case MoveDirectionRight:
		new_column = column_ + 1;
		column_ = (new_column >= MonsterChase::MAX_COLS) ? MonsterChase::MAX_COLS - 1 : new_column;
		break;
	case MoveDirectionUp:
		new_row = row_ - 1;
		row_ = (new_row < 0) ? 0 : new_row;
		break;
	case MoveDirectionDown:
		new_row = row_ + 1;
		row_ = (new_row >= MonsterChase::MAX_ROWS) ? MonsterChase::MAX_ROWS - 1 : new_row;
		break;
	}
}

void Player::Print()
{
	printf("Player %s is at [%d, %d]\n", name_, column_, row_);
}