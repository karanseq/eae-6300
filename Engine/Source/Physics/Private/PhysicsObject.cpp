#include "Physics\PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"

namespace engine {
namespace physics {

// static member initialization
const float PhysicsObject::DEFAULT_MASS = 2.0f;
const float PhysicsObject::DEFAULT_COEFF_DRAG = 0.05f;
const float PhysicsObject::MIN_VELOCITY_LENGTH_SQUARED = 0.000075f;
const float PhysicsObject::MAX_VELOCITY_LENGTH_SQUARED = 6.00f;

PhysicsObject::PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass, float i_drag) : is_awake_(false),
	game_object_(i_game_object),
	mass_(i_mass),
	inverse_mass_(0.0f),
	coeff_drag_(i_drag),
	max_velocity_length_squared_(PhysicsObject::MAX_VELOCITY_LENGTH_SQUARED),
	force_(engine::math::Vec3D::ZERO),
	prev_velocity_(engine::math::Vec3D::ZERO),
	curr_velocity_(engine::math::Vec3D::ZERO),
	max_velocity_(engine::math::Vec3D::ZERO)
{
	// validate inputs
	ASSERT(game_object_);
	// physics objects must have positive mass
	ASSERT(!engine::math::IsNaN(mass_) && !engine::math::FuzzyEquals(mass_, 0.0f));
	ASSERT(!engine::math::IsNaN(coeff_drag_));

	// calculate inverse mass
	inverse_mass_ = 1.0f / mass_;
}

PhysicsObject::~PhysicsObject()
{}

PhysicsObject::PhysicsObject(const PhysicsObject& i_copy) : is_awake_(i_copy.is_awake_),
	game_object_(new engine::gameobject::GameObject(*(i_copy.game_object_))),
	mass_(i_copy.mass_),
	inverse_mass_(i_copy.inverse_mass_),
	coeff_drag_(i_copy.coeff_drag_),
	force_(i_copy.force_),
	prev_velocity_(i_copy.prev_velocity_),
	curr_velocity_(i_copy.curr_velocity_),
	max_velocity_(i_copy.max_velocity_)
{}

void PhysicsObject::Update(float dt)
{
	// don't process if not awake
	if (!is_awake_)
	{
		return;
	}

	// apply drag to force
	force_ += (force_.IsZero()) ? engine::math::Vec3D::ZERO : (force_ * -coeff_drag_);

	// update velocity
	const engine::math::Vec3D new_velocity = curr_velocity_ + force_;
	curr_velocity_ = new_velocity.LengthSquared() < max_velocity_length_squared_ ? new_velocity : curr_velocity_;

	// apply drag to velocity when no force is acting
	if ((curr_velocity_ - prev_velocity_).IsZero())
	{
		curr_velocity_ += (curr_velocity_ * -coeff_drag_);

		// check if stationary
		if (curr_velocity_.LengthSquared() <= MIN_VELOCITY_LENGTH_SQUARED)
		{
			// bring the object to a stop
			curr_velocity_ = engine::math::Vec3D::ZERO;
			prev_velocity_ = engine::math::Vec3D::ZERO;

			// prevent further simulation
			is_awake_ = false;
		}
	}

	// use midpoint numerical integration to calculate new position
	engine::math::Vec3D new_position = game_object_->GetPosition() + ((prev_velocity_ + curr_velocity_) * 0.5f) * dt;

	// save current velocity
	prev_velocity_ = curr_velocity_;

	// update game object
	game_object_->SetPosition(new_position);
}

void PhysicsObject::ApplyImpulse(const engine::math::Vec3D& i_impulse)
{
	// validate input
	if (i_impulse.IsZero())
	{
		return;
	}

	// start processing
	is_awake_ = true;

	// calculate resultant acceleration
	force_ += i_impulse * inverse_mass_;
}

} // namespace physics
} // namespace engine
