#ifndef ENGINE_VEC2D_H_
#define ENGINE_VEC2D_H_

#include <stdio.h>

namespace engine
{
	class Vec2D
	{
	public:
		Vec2D() : x_(0.0f),
			y_(0.0f)
		{}

		Vec2D(float x, float y) : x_(x),
			y_(y)
		{}

		Vec2D(const Vec2D& copy);

		~Vec2D()
		{}

		// accessors and mutators
		inline float x() const { return x_; }
		inline void x(float x) { x_ = x; }
		inline float y() const { return y_; }
		inline void y(float y) { y_ = y; }
		inline void set(float x, float y) { x_ = x; y_ = y; }

		// assignment
		Vec2D& operator=(const Vec2D& vec);

		// arithmetic
		Vec2D operator+(const Vec2D& vec) const;
		Vec2D& operator+=(const Vec2D& vec);
		Vec2D operator-(const Vec2D& vec) const;
		Vec2D& operator-=(const Vec2D& vec);
		Vec2D& operator*=(float scale);

		// relational
		bool operator==(const Vec2D& vec) const;
		bool operator!=(const Vec2D& vec) const;

		// unary
		Vec2D operator-() const;

		// constants
		static const Vec2D ZERO;
		static const Vec2D UNIT;
		static const Vec2D UNIT_X;
		static const Vec2D UNIT_Y;

	protected:
		float x_;
		float y_;
	}; // class Vec2D

} // namespace engine

#endif // ENGINE_VEC2D_H_