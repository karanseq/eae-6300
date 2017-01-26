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
	PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass = DEFAULT_MASS, float i_drag = DEFAULT_COEFF_DRAG);
	~PhysicsObject();

	// copy constructor
	PhysicsObject(const PhysicsObject& i_copy);
	// copy assignment operator
	inline PhysicsObject& operator=(const PhysicsObject& i_copy);

	// functions
	void Update(float dt);
	void ApplyImpulse(const engine::math::Vec3D& i_impulse);

	// accessors and mutators
	inline bool GetIsAwake() const;
	inline void SetIsAwake(bool is_awake);
	inline engine::gameobject::GameObject* GetGameObject() const;
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object);
	inline float GetMass() const;
	inline void SetMass(float i_mass);
	inline float GetDrag() const;
	inline void SetDrag(float i_drag);
	inline const engine::math::Vec3D& GetVelocity() const;
	inline void SetVelocity(const engine::math::Vec3D& i_velocity);

	// constants
	static const float											DEFAULT_MASS;
	static const float											DEFAULT_COEFF_DRAG;
	static const float											MAX_COEFF_DRAG;
	static const float											MIN_VELOCITY_LENGTH_SQUARED;
	static const float											MAX_VELOCITY_LENGTH_SQUARED;

private:
	bool														is_awake_;
	engine::gameobject::GameObject*								game_object_;
	float														mass_;
	float														inverse_mass_;
	float														coeff_drag_;
	engine::math::Vec3D											prev_velocity_;
	engine::math::Vec3D											curr_velocity_;
}; // class PhysicsObject

} // namespace physics
} // namespace engine

#include "PhysicsObject-inl.h"

#endif // PHYSICS_OBJECT_H_
