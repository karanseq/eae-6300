#ifndef ENGINE_VEC2D_H_
#define ENGINE_VEC2D_H_

namespace engine
{
	class Vec2D
	{
	public:
		Vec2D(float x = 0.0f, float y = 0.0f);
		Vec2D(const Vec2D& copy);

		~Vec2D()
		{}

		// accessors and mutators
		inline float x() const;
		inline void x(float x);
		inline float y() const;
		inline void y(float y);
		inline void set(float x = 0.0f, float y = 0.0f);

		// assignment
		inline Vec2D& operator=(const Vec2D& vec);

		// arithmetic
		inline Vec2D operator+(const Vec2D& vec) const;
		inline Vec2D& operator+=(const Vec2D& vec);
		inline Vec2D operator-(const Vec2D& vec) const;
		inline Vec2D& operator-=(const Vec2D& vec);
		inline Vec2D& operator*=(float scale);

		// relational
		inline bool operator==(const Vec2D& vec) const;
		inline bool operator!=(const Vec2D& vec) const;

		// unary
		inline Vec2D operator-() const;

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

#include "Vec2D-inl.h"

#endif // ENGINE_VEC2D_H_