#include "Math\Vec2D.h"

namespace engine {
namespace math {

	const Vec2D Vec2D::ZERO(0.0f, 0.0f);
	const Vec2D Vec2D::UNIT(1.0f, 1.0f);
	const Vec2D Vec2D::UNIT_X(1.0f, 0.0f);
	const Vec2D Vec2D::UNIT_Y(0.0f, 1.0f);

	Vec2D::Vec2D(float i_x, float i_y) : x_(i_x),
		y_(i_y)
	{}

	Vec2D::Vec2D(const Vec2D& i_copy)
	{
		x_ = i_copy.x();
		y_ = i_copy.y();
	}

} // namespace math
} // namespace engine