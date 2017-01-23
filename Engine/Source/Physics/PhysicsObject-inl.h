#include "PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"

namespace engine {
namespace physics {

	inline PhysicsObject& PhysicsObject::operator=(const PhysicsObject& i_copy)
	{
		// check for self assignment
		if (this != &i_copy)
		{
			game_object_ = new engine::gameobject::GameObject(*(i_copy.game_object_));
			mass_ = i_copy.mass_;
			drag_ = i_copy.drag_;
			acceleration_ = i_copy.acceleration_;
			prev_position_ = i_copy.prev_position_;
		}

		return *this;
	}

	inline engine::gameobject::GameObject* PhysicsObject::GetGameObject() const
	{
		return game_object_;
	}

	inline void PhysicsObject::SetGameObject(engine::gameobject::GameObject* i_game_object)
	{
		ASSERT(i_game_object);
		game_object_ = i_game_object;
	}

	inline float PhysicsObject::GetMass() const
	{
		return mass_;
	}

	inline void PhysicsObject::SetMass(float i_mass)
	{
		ASSERT(!engine::math::IsNaN(i_mass) && !engine::math::FuzzyEquals(i_mass, 0.0f));
		mass_ = i_mass;
	}

	inline float PhysicsObject::GetDrag() const
	{
		return drag_;
	}

	inline void PhysicsObject::SetDrag(float i_drag)
	{
		ASSERT(!engine::math::IsNaN(i_drag) && !engine::math::FuzzyEquals(i_drag, 0.0f));
		drag_ = i_drag;
	}

	inline float PhysicsObject::GetAcceleration() const
	{
		return acceleration_;
	}

	inline void PhysicsObject::SetAcceleration(float i_acceleration)
	{
		ASSERT(!engine::math::IsNaN(i_acceleration) && !engine::math::FuzzyEquals(i_acceleration, 0.0f));
		acceleration_ = i_acceleration;
	}

	inline const engine::math::Vec3D& PhysicsObject::GetPreviousPosition() const
	{
		return prev_position_;
	}

	inline void PhysicsObject::SetPreviousPosition(const engine::math::Vec3D& i_prev_position)
	{
		prev_position_ = i_prev_position;
	}

} // namespace physics
} // namespace engine