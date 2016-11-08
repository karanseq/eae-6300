#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Math\Vec2D.h"

namespace engine {

/*
	A simple class to represent an object in space.
*/
class GameObject
{
public:
	virtual ~GameObject() {}

	inline const Vec2D& GetPosition() const					{ return position_; }
	inline void SetPosition(const Vec2D& position)			{ position_ = position; }

private:
	Vec2D position_;
};

} // namespace engine

#endif // GAME_OBJECT_H_
