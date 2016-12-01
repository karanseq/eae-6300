#ifndef MONSTER_H_
#define MONSTER_H_

#include <stdint.h>

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
	virtual void Print() const;

	// accessors and mutators
	inline int GetTimeToLive() const;
	inline void SetTimeToLive(uint8_t time_to_live);

	// constants
	static const uint8_t MAX_MONSTERS = 10;
	static const uint8_t MAX_MONSTER_TTL = 10;

protected:
	// data
	uint8_t time_to_live_;
};

#include "Monster-inl.h"

#endif // MONSTER_H_