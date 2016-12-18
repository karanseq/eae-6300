#ifndef ENGINE_GAME_OBJECT_H_
#define ENGINE_GAME_OBJECT_H_

// engine includes
#include "Math\Transform.h"

namespace engine {
namespace gameobject {

/*
	A simple class to represent an object in space.
*/
class GameObject
{
public:
	GameObject() : transform_()
	{}
	virtual ~GameObject()
	{}


	inline const engine::math::Transform& GetTransform() const							{ return transform_; }
	inline void SetTransform(const engine::math::Transform& i_transform)				{ transform_ = i_transform; }

	inline const engine::math::Vec3D& GetPosition() const								{ return transform_.GetPosition(); }
	inline void SetPosition(const engine::math::Vec3D& i_position)						{ transform_.SetPosition(i_position); }

	inline const engine::math::Vec3D& GetRotation() const								{ return transform_.GetRotation(); }
	inline void SetRotation(const engine::math::Vec3D& i_rotation)						{ transform_.SetRotation(i_rotation); }

	inline const engine::math::Vec3D& GetScale() const									{ return transform_.GetScale(); }
	inline void SetScale(const engine::math::Vec3D& i_scale)							{ transform_.SetScale(i_scale); }

private:
	// disable default copy constructor
	GameObject(const GameObject& i_copy);
	// disable default assignment operator
	inline GameObject& operator=(const GameObject& i_game_object);

private:
	engine::math::Transform			transform_;
}; // class GameObject

} // namespace gameobject
} // namespace engine

#endif // ENGINE_GAME_OBJECT_H_
