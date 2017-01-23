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

PhysicsObject::PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass, float i_drag) : is_awake_(false),
	game_object_(i_game_object),
	mass_(i_mass),
	inverse_mass_(0.0f),
	coeff_drag_(i_drag),
	acceleration_(0.0f),
	prev_position_(engine::math::Vec3D::ZERO)
{
	// validate inputs
	ASSERT(game_object_);
	// physics objects must have positive mass
	ASSERT(!engine::math::IsNaN(mass_) && !engine::math::FuzzyEquals(mass_, 0.0f));
	ASSERT(!engine::math::IsNaN(coeff_drag_));

	// calculate inverse mass
	inverse_mass_ = 1.0f / mass_;

	prev_position_ = game_object_->GetPosition();
}

PhysicsObject::~PhysicsObject()
{}

PhysicsObject::PhysicsObject(const PhysicsObject& i_copy) : is_awake_(i_copy.is_awake_),
	game_object_(new engine::gameobject::GameObject(*(i_copy.game_object_))),
	mass_(i_copy.mass_),
	inverse_mass_(i_copy.inverse_mass_),
	coeff_drag_(i_copy.coeff_drag_),
	acceleration_(i_copy.acceleration_),
	prev_position_(i_copy.prev_position_)
{}

void PhysicsObject::Update(float dt)
{
	if (!is_awake_ || acceleration_ == engine::math::Vec3D::ZERO)
	{
		is_awake_ = false;
		// when stationary, previous position is the same as current
		prev_position_ = game_object_->GetPosition();
		return;
	}

	// save current position
	const engine::math::Vec3D& curr_position = game_object_->GetPosition();

	// apply drag
	acceleration_ += (acceleration_ * -coeff_drag_);
	// error correct
	acceleration_ = (acceleration_ == engine::math::Vec3D::ZERO) ? engine::math::Vec3D::ZERO : acceleration_;

	// use verlet numerical integration to calculate new position
	engine::math::Vec3D new_position = (curr_position * 2.0f) - prev_position_ + acceleration_ * dt;

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

	is_awake_ = true;

	// calculate resultant acceleration
	acceleration_ += i_impulse * inverse_mass_;
}

} // namespace physics
} // namespace engine
