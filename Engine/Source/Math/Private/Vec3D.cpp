#include "Math\Vec3D.h"

namespace engine {
namespace math {

	const Vec3D Vec3D::ZERO(0.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT(1.0f, 1.0f, 1.0f);
	const Vec3D Vec3D::UNIT_X(1.0f, 0.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Y(0.0f, 1.0f, 0.0f);
	const Vec3D Vec3D::UNIT_Z(0.0f, 0.0f, 1.0f);

	Vec3D::Vec3D(float i_x, float i_y, float i_z) : x_(i_x),
		y_(i_y),
		z_(i_z)
	{}

	Vec3D::Vec3D(const Vec3D& i_copy) : x_(i_copy.x_),
		y_(i_copy.y_),
		z_(i_copy.z_)
	{}

} // namespace math
} // namespace engine