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

	float Vec2D::x() const
	{ 
		return x_; 
	}

	void Vec2D::x(float x)
	{ 
		x_ = x;
	}
	
	float Vec2D::y() const
	{ 
		return y_;
	}

	void Vec2D::y(float y)
	{ 
		y_ = y;
	}
	
	void Vec2D::set(float x, float y)
	{ 
		x_ = x; 
		y_ = y; 
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

	Vec2D& Vec2D::operator*=(float scale)
	{
		x(x_ * scale);
		y(y_ * scale);
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