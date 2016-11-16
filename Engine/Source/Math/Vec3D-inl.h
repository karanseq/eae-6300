#include "Vec3D.h"

namespace engine
{
	inline float Vec3D::x() const
	{
		return x_;
	}

	inline void Vec3D::x(float x)
	{
		x_ = x;
	}

	inline float Vec3D::y() const
	{
		return y_;
	}

	inline void Vec3D::y(float y)
	{
		y_ = y;
	}

	inline float Vec3D::z() const
	{
		return z_;
	}

	inline void Vec3D::z(float z)
	{
		z_ = z;
	}

	inline void Vec3D::set(float x, float y, float z)
	{
		x_ = x;
		y_ = y;
		z_ = z;
	}

	inline Vec3D& Vec3D::operator=(const Vec3D& vec)
	{
		// check for self assignment
		if (this == &vec)
		{
			return *this;
		}

		x_ = vec.x_;
		y_ = vec.y_;
		z_ = vec.z_;
		return *this;
	}

	inline Vec3D Vec3D::operator+(const Vec3D& vec) const
	{
		return Vec3D(x_ + vec.x(), y_ + vec.y(), z_ + vec.z());
	}

	inline Vec3D& Vec3D::operator+=(const Vec3D& vec)
	{
		x(x_ + vec.x());
		y(y_ + vec.y());
		z(z_ + vec.z());
		return *this;
	}

	inline Vec3D Vec3D::operator-(const Vec3D& vec) const
	{
		return Vec3D(x_ - vec.x(), y_ - vec.y(), z_ - vec.z());
	}

	inline Vec3D& Vec3D::operator-=(const Vec3D& vec)
	{
		x(x_ - vec.x());
		y(y_ - vec.y());
		z(z_ - vec.z());
		return *this;
	}

	inline Vec3D& Vec3D::operator*=(float scale)
	{
		x(x_ * scale);
		y(y_ * scale);
		z(z_ * scale);
		return *this;
	}

	inline bool Vec3D::operator==(const Vec3D& vec) const
	{
		return (x_ == vec.x() && y_ == vec.y() && z_ == vec.z());
	}

	inline bool Vec3D::operator!=(const Vec3D& vec) const
	{
		return (x_ != vec.x() || y_ != vec.y() || z_ != vec.z());
	}

	inline Vec3D Vec3D::operator-() const
	{
		return Vec3D(-x(), -y(), -z());
	}

} // namespace engine
