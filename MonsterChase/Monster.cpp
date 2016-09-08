#include "Monster.h"
#include "MonsterChase.h"

// include libraries
#include <stdlib.h>
#include <stdio.h>

Monster::Monster()
{
	SetTimeToLive(MAX_MONSTER_TTL / 2 + rand() % MAX_MONSTER_TTL);
}

Monster::Monster(int row, int column, const char* name)
{
	SetRow(row);
	SetColumn(column);
	SetName(name);
	SetTimeToLive(MAX_MONSTER_TTL / 2 + rand() % MAX_MONSTER_TTL);
}

Monster::~Monster()
{}

void Monster::Move(int player_row, int player_column)
{
	if (row_ < player_row)
	{
		Player::Move(MoveDirectionDown);
	}
	else if (row_ > player_row)
	{
		Player::Move(MoveDirectionUp);
	}
	else if (column_ < player_column)
	{
		Player::Move(MoveDirectionRight);
	}
	else if (column_ > player_column)
	{
		Player::Move(MoveDirectionLeft);
	}
	else
	{
		int direction = rand() % 4;
		Player::Move((MoveDirection)direction);
	}

	--time_to_live_;
}

void Monster::Print()
{
	printf("Monster %s %s [%d, %d]\n", name_, (time_to_live_ <= 1 ? "is about to die at" : "is at"), column_, row_);
}