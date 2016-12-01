#ifndef PLAYER_H_
#define PLAYER_H_

#include <string.h>
#include <stdint.h>

#include "Math\Vec2D.h"
#include "Logger\Logger.h"

// forward declaration
enum MoveDirection;

class Player
{
public:
	Player();
	Player(const engine::Vec2D& position, const char* name);
	virtual ~Player();

	// player behavior
	void Move(const MoveDirection move_direction);
	virtual void Print() const;

	// accessors and mutators
	inline const engine::Vec2D& GetPosition() const;
	inline void SetPosition(const engine::Vec2D& position);

	inline const char* GetName() const;
	inline void SetName(const char* name);

	// constants
	static const uint8_t MAX_NAME_LENGTH = 10;

protected:
	// data
	engine::Vec2D position_;
	char name_[MAX_NAME_LENGTH + 1];
};	// class Player

#include "Player-inl.h"

#endif // PLAYER_H_