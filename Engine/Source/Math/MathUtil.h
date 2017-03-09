#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

// library includes
#include <math.h>

#define M_PI					3.14159265358979323846f  /* pi */
#define MIN_EPSILON				0.000000001f
#define MAX_EPSILON				0.0001f

namespace engine {
namespace math {

// forward declarations
class Vec2D;
class Vec3D;
class Vec4D;

inline float RadiansToDegrees(float i_radians)
{
	return (i_radians * 180.0f / M_PI);
}

inline float DegreesToRadians(float i_degrees)
{
	return (i_degrees * M_PI / 180.0f);
}

inline bool IsNaN(float i_number)
{
	volatile float temp = i_number;
	return (temp != i_number);
}

inline bool FuzzyEquals(float i_lhs, float i_rhs, float i_epsilon = MAX_EPSILON)
{
	return fabs(i_rhs - i_lhs) < i_epsilon;
}

inline bool IsZero(float i_number)
{
	return FuzzyEquals(i_number, MIN_EPSILON);
}

// dot products
float DotProduct(const Vec2D& i_v1, const Vec2D& i_v2);
float DotProduct(const Vec3D& i_v1, const Vec3D& i_v2);

// cross product
Vec3D CrossProduct(const Vec3D& i_v1, const Vec3D& i_v2);

} // namespace math
} // namespace engine

#endif // MATH_UTIL_H_

