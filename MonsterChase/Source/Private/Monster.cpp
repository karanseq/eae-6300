#include "Monster.h"
#include "MonsterChase.h"

// include libraries
#include <stdlib.h>
#include <stdio.h>

Monster::Monster()
{
	SetTimeToLive(MAX_MONSTER_TTL / 2 + rand() % MAX_MONSTER_TTL);
}

Monster::Monster(const engine::Vec2D& position, const char* name)
{
	SetPosition(position);
	SetName(name);
	SetTimeToLive(MAX_MONSTER_TTL / 2 + rand() % MAX_MONSTER_TTL);
}

Monster::~Monster()
{}

void Monster::Move(const engine::Vec2D& player_position)
{
	if (position_.y() < player_position.y())
	{
		Player::Move(MoveDirectionUp);
	}
	else if (position_.y() > player_position.y())
	{
		Player::Move(MoveDirectionDown);
	}
	else if (position_.x() < player_position.x())
	{
		Player::Move(MoveDirectionRight);
	}
	else if (position_.x() > player_position.x())
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

void Monster::Print() const
{
	printf("Monster %s %s [%f, %f]\n", name_, (time_to_live_ <= 1 ? "is about to die at" : "is at"), position_.x(), position_.y());
}