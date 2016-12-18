#ifndef ENGINE_TRANSFORM_H_
#define ENGINE_TRANSFORM_H_

#include "Vec3D.h"

namespace engine {
namespace math {

	class Transform
	{
	public:
		Transform(Vec3D i_position = Vec3D::ZERO, Vec3D i_rotation = Vec3D::ZERO, Vec3D i_scale = Vec3D::UNIT);
		Transform(const Transform& i_copy);

		~Transform()
		{}

		// assignment
		inline Transform& operator=(const Transform& i_transform);

		// accessors and mutators
		inline const Vec3D& GetPosition() const;
		inline void SetPosition(const Vec3D& i_position);
		inline const Vec3D& GetRotation() const;
		inline void SetRotation(const Vec3D& i_rotation);
		inline const Vec3D& GetScale() const;
		inline void SetScale(const Vec3D& i_scale);

	protected:
		Vec3D position_;
		Vec3D rotation_;
		Vec3D scale_;

	}; // class Transform

} // namespace math
} // namespace engine

#include "Transform-inl.h"

#endif // ENGINE_TRANSFORM_H_
