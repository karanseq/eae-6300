#include "Math\Vec2D.h"

namespace engine
{
	Vec2D::Vec2D(const Vec2D& copy)
	{
		x_ = copy.x();
		y_ = copy.y();
	}

	Vec2D& Vec2D::operator=(const Vec2D& vec)
	{
		// check for self assignment
		if (this == &vec)
		{
			return *this;
		}

		x(vec.x());
		y(vec.y());
		return *this;
	}

	Vec2D Vec2D::operator+(const Vec2D& vec) const
	{
		return Vec2D(x_ + vec.x(), y_ + vec.y());
	}

	Vec2D& Vec2D::operator+=(const Vec2D& vec)
	{
		x(x_ + vec.x());
		y(y_ + vec.y());
		return *this;
	}

	Vec2D Vec2D::operator-(const Vec2D& vec) const
	{
		return Vec2D(x_ - vec.x(), y_ - vec.y());
	}

	Vec2D& Vec2D::operator-=(const Vec2D& vec)
	{
		x(x_ - vec.x());
		y(y_ - vec.y());
		return *this;
	}

	bool Vec2D::operator==(const Vec2D& vec) const
	{
		return (x_ == vec.x() && y_ == vec.y());
	}

	bool Vec2D::operator!=(const Vec2D& vec) const
	{
		return (x_ != vec.x() || y_ != vec.y());
	}

	Vec2D Vec2D::operator-() const
	{
		return Vec2D(-x(), -y());
	}

} // namespace engine