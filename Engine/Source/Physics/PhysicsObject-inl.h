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
			inverse_mass_ = i_copy.inverse_mass_;
			coeff_drag_ = i_copy.coeff_drag_;
			force_ = i_copy.force_;
			prev_velocity_ = i_copy.prev_velocity_;
			curr_velocity_ = i_copy.curr_velocity_;
			max_velocity_ = i_copy.max_velocity_;
		}

		return *this;
	}

	inline bool PhysicsObject::GetIsAwake() const
	{
		return is_awake_;
	}

	inline void PhysicsObject::SetIsAwake(bool is_awake)
	{
		is_awake_ = is_awake;
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
		// physics objects must have positive mass
		ASSERT(!engine::math::IsNaN(i_mass) && !engine::math::FuzzyEquals(i_mass, 0.0f));
		mass_ = i_mass;
		inverse_mass_ = 1.0f / mass_;
	}

	inline float PhysicsObject::GetDrag() const
	{
		return coeff_drag_;
	}

	inline void PhysicsObject::SetDrag(float i_drag)
	{
		ASSERT(!engine::math::IsNaN(i_drag));
		coeff_drag_ = i_drag;
	}

	inline const engine::math::Vec3D& PhysicsObject::GetForce() const
	{
		return force_;
	}

	inline void PhysicsObject::SetForce(const engine::math::Vec3D& i_force)
	{
		force_ = i_force;
	}

	inline const engine::math::Vec3D& PhysicsObject::GetVelocity() const
	{
		return curr_velocity_;
	}

	inline void PhysicsObject::SetVelocity(const engine::math::Vec3D& i_velocity)
	{
		curr_velocity_ = i_velocity;
	}

	inline const engine::math::Vec3D& PhysicsObject::GetMaxVelocity() const
	{
		return max_velocity_;
	}

	inline void PhysicsObject::SetMaxVelocity(const engine::math::Vec3D& i_max_velocity)
	{
		max_velocity_ = i_max_velocity;
		max_velocity_length_squared_ = max_velocity_.LengthSquared();
	}

} // namespace physics
} // namespace engine