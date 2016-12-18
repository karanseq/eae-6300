#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#include <math.h>

#define M_PI					3.14159265358979323846f  /* pi */
#define MIN_EPSILON				0.000000001f
#define MAX_EPSILON				0.0001f

namespace engine {
namespace math {

	inline float RadiansToDegrees(float radians)
	{
		return (radians * 180.0f / M_PI);
	}

	inline float DegreesToRadians(float degrees)
	{
		return (degrees * M_PI / 180.0f);
	}

	inline bool IsNaN(float number)
	{
		volatile float temp = number;
		return (temp != number);
	}

	inline bool FuzzyEqual(float lhs, float rhs, float epsilon = MAX_EPSILON)
	{
		return fabs(rhs - lhs) < epsilon;
	}

	inline bool IsZero(float number)
	{
		return FuzzyEqual(number, MIN_EPSILON);
	}

} // namespace math
} // namespace engine

#endif // MATH_UTIL_H_

