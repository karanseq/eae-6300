#include "Vec2D.h"
#include "Assert\Assert.h"
#include "Math\MathUtil.h"

namespace engine {
namespace math {

	inline float Vec2D::x() const
	{
		return x_;
	}

	inline void Vec2D::x(float i_x)
	{
		ASSERT(!IsNaN(i_x));
		x_ = i_x;
	}

	inline float Vec2D::y() const
	{
		return y_;
	}

	inline void Vec2D::y(float i_y)
	{
		ASSERT(!IsNaN(i_y));
		y_ = i_y;
	}

	inline void Vec2D::set(float i_x, float i_y)
	{
		ASSERT(!IsNaN(i_x) && !IsNaN(i_y));
		x_ = i_x;
		y_ = i_y;
	}

	inline Vec2D& Vec2D::operator=(const Vec2D& i_vec)
	{
		// check for self assignment
		if (this == &i_vec)
		{
			return *this;
		}

		x_ = i_vec.x_;
		y_ = i_vec.y_;
		return *this;
	}

	inline Vec2D Vec2D::operator+(const Vec2D& i_vec) const
	{
		return Vec2D(x_ + i_vec.x(), y_ + i_vec.y());
	}

	inline Vec2D& Vec2D::operator+=(const Vec2D& i_vec)
	{
		x(x_ + i_vec.x());
		y(y_ + i_vec.y());
		return *this;
	}

	inline Vec2D Vec2D::operator-(const Vec2D& i_vec) const
	{
		return Vec2D(x_ - i_vec.x(), y_ - i_vec.y());
	}

	inline Vec2D& Vec2D::operator-=(const Vec2D& i_vec)
	{
		x(x_ - i_vec.x());
		y(y_ - i_vec.y());
		return *this;
	}

	inline Vec2D& Vec2D::operator*=(float i_scale)
	{
		ASSERT(!IsNaN(i_scale));
		x(x_ * i_scale);
		y(y_ * i_scale);
		return *this;
	}

	inline bool Vec2D::operator==(const Vec2D& i_vec) const
	{
		return (FuzzyEqual(x_, i_vec.x()) && FuzzyEqual(y_, i_vec.y()));
	}

	inline bool Vec2D::operator!=(const Vec2D& i_vec) const
	{
		return (x_ != i_vec.x() || y_ != i_vec.y());
	}

	inline Vec2D Vec2D::operator-() const
	{
		return Vec2D(-x(), -y());
	}

} // namespace math
} // namespace engine