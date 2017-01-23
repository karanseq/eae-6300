#include "Physics\PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"
#include "Math\MathUtil.h"

namespace engine {
namespace physics {

const float PhysicsObject::DEFAULT_COEFF_DRAG = 0.1f;

PhysicsObject::PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass, float i_drag) : game_object_(i_game_object),
	mass_(i_mass),
	coeff_drag_(i_drag),
	acceleration_(0.0f),
	prev_position_(engine::math::Vec3D::ZERO)
{
	// validate inputs
	ASSERT(game_object_);
	ASSERT(!engine::math::IsNaN(mass_) && !engine::math::FuzzyEquals(mass_, 0.0f));
	ASSERT(!engine::math::IsNaN(coeff_drag_) && !engine::math::FuzzyEquals(coeff_drag_, 0.0f));

	prev_position_ = game_object_->GetPosition();
}

PhysicsObject::~PhysicsObject()
{}

PhysicsObject::PhysicsObject(const PhysicsObject& i_copy) : game_object_(new engine::gameobject::GameObject(*(i_copy.game_object_))),
	mass_(i_copy.mass_),
	coeff_drag_(i_copy.coeff_drag_),
	acceleration_(i_copy.acceleration_),
	prev_position_(i_copy.prev_position_)
{}

void PhysicsObject::Update(float dt)
{
	if (acceleration_ == engine::math::Vec3D::ZERO)
	{
		return;
	}

	// save current position
	const engine::math::Vec3D& curr_position = game_object_->GetPosition();

	// use verlet numerical integration to calculate new position
	engine::math::Vec3D new_position = (curr_position * 2.0f) - prev_position_ + acceleration_ * dt;

	// apply drag
	acceleration_ += (acceleration_ * -coeff_drag_);
	// error correct
	acceleration_ = acceleration_ == engine::math::Vec3D::ZERO ? engine::math::Vec3D::ZERO : acceleration_;

	// save current position
	prev_position_ = curr_position;
	// update game object
	game_object_->SetPosition(new_position);
}

void PhysicsObject::ApplyImpulse(const engine::math::Vec3D& i_impulse)
{
	// validate input
	if (i_impulse == engine::math::Vec3D::ZERO)
	{
		return;
	}

	// calculate resultant acceleration
	acceleration_ = i_impulse * mass_;
}

} // namespace physics
} // namespace engine
