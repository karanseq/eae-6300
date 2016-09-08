#pragma once
#include "Player.h"

class Monster : public Player
{
public:
	Monster();
	Monster(int row, int column, const char* name);
	virtual ~Monster();

	// monster behavior
	void Move(int player_row, int player_column);
	virtual void Print();

	// accessors and mutators
	inline int GetTimeToLive() { return time_to_live_; }
	inline void SetTimeToLive(int time_to_live) { time_to_live_ = time_to_live; }

	// constants
	static const int MAX_MONSTERS = 10;
	static const int MAX_MONSTER_TTL = 10;

protected:
	// data
	int time_to_live_;
};

