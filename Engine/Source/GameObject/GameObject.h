#ifndef ENGINE_GAME_OBJECT_H_
#define ENGINE_GAME_OBJECT_H_

// engine includes
#include "Math\Transform.h"

namespace engine {

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


	inline const Transform& GetTransform() const					{ return transform_; }
	inline void SetTransform(const Transform& transform)			{ transform_ = transform; }

	inline const Vec3D& GetPosition() const							{ return transform_.GetPosition(); }
	inline void SetPosition(const Vec3D& position)					{ transform_.SetPosition(position); }

	inline const Vec3D& GetRotation() const							{ return transform_.GetRotation(); }
	inline void SetRotation(const Vec3D& rotation)					{ transform_.SetRotation(rotation); }

	inline const Vec3D& GetScale() const							{ return transform_.GetScale(); }
	inline void SetScale(const Vec3D& scale)						{ transform_.SetScale(scale); }

private:
	// disable default copy constructor
	GameObject(const GameObject& copy);
	// disable default assignment operator
	inline GameObject& operator=(const GameObject& game_object);

private:
	Transform transform_;
}; // class GameObject

} // namespace engine

#endif // ENGINE_GAME_OBJECT_H_
