#ifndef MONSTER_H_
#define MONSTER_H_

#include "Player.h"

class engine::Vec2D;

class Monster : public Player
{
public:
	Monster();
	Monster(const engine::Vec2D& position, const char* name);
	virtual ~Monster();

	// monster behavior
	void Move(const engine::Vec2D& player_position);
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

#endif // MONSTER_H_