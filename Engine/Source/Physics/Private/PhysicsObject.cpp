#include "Physics\PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"
#include "Math\MathUtil.h"

namespace engine {
namespace physics {

const float PhysicsObject::DEFAULT_DRAG = 1.0f;

PhysicsObject::PhysicsObject(engine::gameobject::GameObject* i_game_object, float i_mass, float i_drag) : game_object_(i_game_object),
	mass_(i_mass),
	drag_(i_drag),
	acceleration_(0.0f),
	prev_position_(engine::math::Vec3D::ZERO)
{
	// validate inputs
	ASSERT(game_object_);
	ASSERT(!engine::math::IsNaN(mass_) && !engine::math::FuzzyEquals(mass_, 0.0f));
	ASSERT(!engine::math::IsNaN(drag_) && !engine::math::FuzzyEquals(drag_, 0.0f));

	prev_position_ = game_object_->GetPosition();
}

PhysicsObject::~PhysicsObject()
{}

PhysicsObject::PhysicsObject(const PhysicsObject& i_copy) : game_object_(new engine::gameobject::GameObject(*(i_copy.game_object_))),
	mass_(i_copy.mass_),
	drag_(i_copy.drag_),
	acceleration_(i_copy.acceleration_),
	prev_position_(i_copy.prev_position_)
{}

} // namespace physics
} // namespace engine
