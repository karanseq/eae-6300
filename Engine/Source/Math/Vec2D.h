#ifndef ENGINE_VEC2D_H_
#define ENGINE_VEC2D_H_

#include <stdio.h>

namespace engine
{
	class Vec2D
	{
	public:
		Vec2D();
		Vec2D(float x, float y);
		Vec2D(const Vec2D& copy);

		~Vec2D()
		{}

		// accessors and mutators
		float x() const;
		void x(float x);
		float y() const;
		void y(float y);
		void set(float x, float y);

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