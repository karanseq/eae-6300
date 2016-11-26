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


	inline const engine::math::Transform& GetTransform() const						{ return transform_; }
	inline void SetTransform(const engine::math::Transform& transform)				{ transform_ = transform; }

	inline const engine::math::Vec3D& GetPosition() const							{ return transform_.GetPosition(); }
	inline void SetPosition(const engine::math::Vec3D& position)					{ transform_.SetPosition(position); }

	inline const engine::math::Vec3D& GetRotation() const							{ return transform_.GetRotation(); }
	inline void SetRotation(const engine::math::Vec3D& rotation)					{ transform_.SetRotation(rotation); }

	inline const engine::math::Vec3D& GetScale() const								{ return transform_.GetScale(); }
	inline void SetScale(const engine::math::Vec3D& scale)							{ transform_.SetScale(scale); }

private:
	// disable default copy constructor
	GameObject(const GameObject& copy);
	// disable default assignment operator
	inline GameObject& operator=(const GameObject& game_object);

private:
	engine::math::Transform			transform_;
}; // class GameObject

} // namespace gameobject
} // namespace engine

#endif // ENGINE_GAME_OBJECT_H_
