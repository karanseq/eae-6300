#include "Vec2D.h"

namespace engine
{
	inline float Vec2D::x() const
	{
		return x_;
	}

	inline void Vec2D::x(float x)
	{
		x_ = x;
	}

	inline float Vec2D::y() const
	{
		return y_;
	}

	inline void Vec2D::y(float y)
	{
		y_ = y;
	}

	inline void Vec2D::set(float x, float y)
	{
		x_ = x;
		y_ = y;
	}

	inline Vec2D& Vec2D::operator=(const Vec2D& vec)
	{
		// check for self assignment
		if (this == &vec)
		{
			return *this;
		}

		x_ = vec.x_;
		y_ = vec.y_;
		return *this;
	}

	inline Vec2D Vec2D::operator+(const Vec2D& vec) const
	{
		return Vec2D(x_ + vec.x(), y_ + vec.y());
	}

	inline Vec2D& Vec2D::operator+=(const Vec2D& vec)
	{
		x(x_ + vec.x());
		y(y_ + vec.y());
		return *this;
	}

	inline Vec2D Vec2D::operator-(const Vec2D& vec) const
	{
		return Vec2D(x_ - vec.x(), y_ - vec.y());
	}

	inline Vec2D& Vec2D::operator-=(const Vec2D& vec)
	{
		x(x_ - vec.x());
		y(y_ - vec.y());
		return *this;
	}

	inline Vec2D& Vec2D::operator*=(float scale)
	{
		x(x_ * scale);
		y(y_ * scale);
		return *this;
	}

	inline bool Vec2D::operator==(const Vec2D& vec) const
	{
		return (x_ == vec.x() && y_ == vec.y());
	}

	inline bool Vec2D::operator!=(const Vec2D& vec) const
	{
		return (x_ != vec.x() || y_ != vec.y());
	}

	inline Vec2D Vec2D::operator-() const
	{
		return Vec2D(-x(), -y());
	}

} // namespace engine