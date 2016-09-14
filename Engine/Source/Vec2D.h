#pragma once

#include <stdio.h>

namespace engine
{
	class Vec2D
	{
	public:
		Vec2D() : x_(0.0f),
			y_(0.0f)
		{
			printf("%s.h::%d\n", __FUNCTION__, __LINE__);
		}

		Vec2D(float x, float y) : x_(x),
			y_(y)
		{
			printf("%s.h::%d\n", __FUNCTION__, __LINE__);
		}

		Vec2D(const Vec2D& copy);

		~Vec2D()
		{
			printf("%s.h::%d\n", __FUNCTION__, __LINE__);
		}

		inline float x() const { return x_; }
		inline void x(float x) { x_ = x; }
		inline float y() const { return y_; }
		inline void y(float y) { y_ = y; }

		Vec2D& operator=(const Vec2D& vec);
		Vec2D operator+(const Vec2D& vec) const;
		Vec2D& operator+=(const Vec2D& vec);
		Vec2D operator-(const Vec2D& vec) const;
		Vec2D& operator-=(const Vec2D& vec);
		bool operator==(const Vec2D& vec) const;
		bool operator!=(const Vec2D& vec) const;

	protected:
		float x_;
		float y_;
	}; // class Vec2D

	//inline Vec2D operator+(const Vec2D& vec1, const Vec2D& vec2);
	//inline Vec2D operator-(const Vec2D& vec1, const Vec2D& vec2);

} // namespace engine
