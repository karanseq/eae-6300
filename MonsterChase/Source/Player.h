#ifndef PLAYER_H_
#define PLAYER_H_

#include <string.h>
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
	void Move(MoveDirection move_direction);
	virtual void Print() const;

	// accessors and mutators
	inline const engine::Vec2D& GetPosition() const { return position_; }
	inline void SetPosition(const engine::Vec2D& position) { position_ = position; }

	inline const char* GetName() const { return name_; }
	inline void SetName(const char* name) { ASSERT(name != NULL); strcpy_s(name_, name); }

	// constants
	static const int MAX_NAME_LENGTH = 10;

protected:
	// data
	engine::Vec2D position_;
	char name_[MAX_NAME_LENGTH + 1];
};	// class Player

#endif // PLAYER_H_