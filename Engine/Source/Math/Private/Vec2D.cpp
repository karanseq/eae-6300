#include "Math\Vec2D.h"

namespace engine
{
	const Vec2D Vec2D::ZERO(0.0f, 0.0f);
	const Vec2D Vec2D::UNIT(1.0f, 1.0f);
	const Vec2D Vec2D::UNIT_X(1.0f, 0.0f);
	const Vec2D Vec2D::UNIT_Y(0.0f, 1.0f);

	Vec2D::Vec2D() : x_(0.0f),
		y_(0.0f)
	{}

	Vec2D::Vec2D(float x, float y) : x_(x),
		y_(y)
	{}

	Vec2D::Vec2D(const Vec2D& copy)
	{
		x_ = copy.x();
		y_ = copy.y();
	}

} // namespace engine