#ifndef PHYSICS_OBJECT_H_
#define PHYSICS_OBJECT_H_

// engine includes
#include "Math\Vec3D.h"

// forward declaration
namespace engine {
namespace gameobject {
	class GameObject;
}
}

namespace engine {
namespace physics {

/*
	PhysicsObject
	- A class that can be used to associate physics with a game object
*/

class PhysicsObject
{
public:
	PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass, float i_drag = DEFAULT_DRAG);
	~PhysicsObject();

	// copy constructor
	PhysicsObject(const PhysicsObject& i_copy);
	// copy assignment operator
	inline PhysicsObject& operator=(const PhysicsObject& i_copy);

	// accessors and mutators
	inline engine::gameobject::GameObject* GetGameObject() const;
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object);
	inline float GetMass() const;
	inline void SetMass(float i_mass);
	inline float GetDrag() const;
	inline void SetDrag(float i_drag);
	inline float GetAcceleration() const;
	inline void SetAcceleration(float i_acceleration);
	inline const engine::math::Vec3D& GetPreviousPosition() const;
	inline void SetPreviousPosition(const engine::math::Vec3D& i_prev_position);

	// constants
	static const float											DEFAULT_DRAG;

private:
	engine::gameobject::GameObject*								game_object_;
	float														mass_;
	float														drag_;
	float														acceleration_;
	engine::math::Vec3D											prev_position_;
}; // class PhysicsObject

} // namespace physics
} // namespace engine

#include "PhysicsObject-inl.h"

#endif // PHYSICS_OBJECT_H_
