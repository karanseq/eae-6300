#include "Transform.h"

namespace engine
{
	inline const Vec3D& Transform::GetPosition() const
	{
		return position_;
	}

	inline void Transform::SetPosition(const Vec3D& position)
	{
		position_ = position;
	}

	inline const Vec3D& Transform::GetRotation() const
	{
		return rotation_;
	}

	inline void Transform::SetRotation(const Vec3D& rotation)
	{
		rotation_ = rotation;
	}

	inline const Vec3D& Transform::GetScale() const
	{
		return scale_;
	}

	inline void Transform::SetScale(const Vec3D& scale)
	{
		scale_ = scale;
	}

} // namespace engine