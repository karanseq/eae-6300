#include "Vec2D.h"

namespace engine
{
	Vec2D::Vec2D(const Vec2D& copy)
	{
		x_ = copy.x();
		y_ = copy.y();
		printf("%s::%d\n", __FUNCTION__, __LINE__);
	}

	Vec2D& Vec2D::operator=(const Vec2D& vec)
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
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
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		return Vec2D(x_ + vec.x(), y_ + vec.y());
	}

	Vec2D& Vec2D::operator+=(const Vec2D& vec)
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		x(x_ + vec.x());
		y(y_ + vec.y());
		return *this;
	}

	Vec2D Vec2D::operator-(const Vec2D& vec) const
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		return Vec2D(x_ - vec.x(), y_ - vec.y());
	}

	Vec2D& Vec2D::operator-=(const Vec2D& vec)
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		x(x_ - vec.x());
		y(y_ - vec.y());
		return *this;
	}

	bool Vec2D::operator==(const Vec2D& vec) const
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		return (x_ == vec.x() && y_ == vec.y());
	}

	bool Vec2D::operator!=(const Vec2D& vec) const
	{
		printf("%s::%d\n", __FUNCTION__, __LINE__);
		return (x_ != vec.x() || y_ != vec.y());
	}

	//Vec2D operator+(const Vec2D& vec1, const Vec2D& vec2)
	//{
	//	printf("%s::%d\n", __FUNCTION__, __LINE__);
	//	return Vec2D(vec1.x() + vec2.x(), vec1.y() + vec2.y());
	//}

	//Vec2D operator-(const Vec2D& vec1, const Vec2D& vec2)
	//{
	//	printf("%s::%d\n", __FUNCTION__, __LINE__);
	//	return Vec2D(vec1.x() - vec2.x(), vec1.y() - vec2.y());
	//}
}