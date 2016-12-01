#include "Player.h"

inline const engine::Vec2D& Player::GetPosition() const
{
	return position_;
}

inline void Player::SetPosition(const engine::Vec2D& position)
{
	position_ = position;
}

inline const char* Player::GetName() const
{
	return name_;
}

inline void Player::SetName(const char* name)
{
	ASSERT(name != NULL);
	strcpy_s(name_, name);
}