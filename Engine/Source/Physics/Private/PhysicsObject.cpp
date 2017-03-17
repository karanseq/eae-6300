#include "Physics\PhysicsObject.h"

// engine includes
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"
#include "Memory\SharedPointer.h"

#ifdef ENABLE_DEBUG_DRAW
#include "Math\AABB.h"
#include "Math\Mat44.h"
#include "Renderer\Renderer.h"
#endif

namespace engine {
namespace physics {

// static member initialization
const float PhysicsObject::DEFAULT_MASS = 2.0f;
const float PhysicsObject::DEFAULT_COEFF_DRAG = 0.025f;
const float PhysicsObject::MAX_COEFF_DRAG = 0.9f;
const float PhysicsObject::MIN_VELOCITY_LENGTH_SQUARED = 0.000075f;
const float PhysicsObject::MAX_VELOCITY_LENGTH_SQUARED = 6.00f;

PhysicsObject::PhysicsObject(const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object, float i_mass, float i_drag, bool i_is_collidable) : is_awake_(false),
    is_collidable_(i_is_collidable),
	game_object_(i_game_object),
	mass_(i_mass),
	inverse_mass_(0.0f),
	coeff_drag_(i_drag),
	curr_velocity_(engine::math::Vec3D::ZERO)
{
	// validate inputs
	ASSERT(game_object_);
	// physics objects must have positive mass
	ASSERT(!engine::math::IsNaN(mass_) && !engine::math::FuzzyEquals(mass_, 0.0f));
	ASSERT(!engine::math::IsNaN(coeff_drag_));

	// calculate inverse mass
	inverse_mass_ = 1.0f / mass_;

#ifdef ENABLE_DEBUG_DRAW
    debug_draw_data_ = nullptr;
#endif
}

PhysicsObject::~PhysicsObject()
{
#ifdef ENABLE_DEBUG_DRAW
    SAFE_DELETE(debug_draw_data_);
#endif
}

PhysicsObject::PhysicsObject(const PhysicsObject& i_copy) : is_awake_(i_copy.is_awake_),
	game_object_(i_copy.game_object_),
	mass_(i_copy.mass_),
	inverse_mass_(i_copy.inverse_mass_),
	coeff_drag_(i_copy.coeff_drag_),
	curr_velocity_(i_copy.curr_velocity_)
{
#ifdef ENABLE_DEBUG_DRAW
    debug_draw_data_ = nullptr;
#endif
}

void PhysicsObject::Update(float i_dt)
{
	// don't process if not awake
	if (!is_awake_)
	{
		return;
	}

	// save previous velocity
	engine::math::Vec3D prev_velocity = curr_velocity_;

	// apply drag to velocity when no force is acting
	curr_velocity_ += (curr_velocity_ * -coeff_drag_);

	// check if stationary
	if (curr_velocity_.LengthSquared() <= MIN_VELOCITY_LENGTH_SQUARED)
	{
		// bring the object to a stop
		curr_velocity_ = engine::math::Vec3D::ZERO;
		prev_velocity = engine::math::Vec3D::ZERO;

		// prevent further simulation
		is_awake_ = false;
	}

	// get a shared pointer to operate on
	engine::memory::SharedPointer<engine::gameobject::GameObject> game_object(game_object_);

	// use midpoint numerical integration to calculate new position
	engine::math::Vec3D new_position = game_object->GetPosition() + ((prev_velocity + curr_velocity_) * 0.5f) * i_dt;

	// update game object
	game_object->SetPosition(new_position);

#ifdef ENABLE_DEBUG_DRAW
    DrawDebugData(i_dt);
#endif
}

#ifdef ENABLE_DEBUG_DRAW
void PhysicsObject::InitDebugDraw(uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a)
{
    debug_draw_data_ = new DebugDrawData(i_r, i_g, i_b, i_a);
    ASSERT(debug_draw_data_);
    DrawDebugData(0.0f);
}

void PhysicsObject::DrawDebugData(float i_dt)
{
    if (!debug_draw_data_)
    {
        return;
    }

    // get a reference to the game object
    const engine::memory::SharedPointer<engine::gameobject::GameObject> game_object = game_object_.Lock();

    // calculate transform
    engine::math::Mat44 mat_transformation;
    engine::math::GetObjectToWorldTransform(game_object->GetTransform(), mat_transformation);
    
    // update the debug draw data
    debug_draw_data_->Update(i_dt, game_object->GetAABB(), mat_transformation);
}
#endif // ENABLE_DEBUG_DRAW

void PhysicsObject::ApplyImpulse(const engine::math::Vec3D& i_impulse)
{
	// validate input
	if (i_impulse.IsZero())
	{
		return;
	}

	// start processing
	is_awake_ = true;

	// calculate new velocity
	engine::math::Vec3D new_velocity = curr_velocity_ + (i_impulse * inverse_mass_);
	// limit max velocity
	curr_velocity_ = new_velocity.LengthSquared() > MAX_VELOCITY_LENGTH_SQUARED ? curr_velocity_ : new_velocity;
}

} // namespace physics
} // namespace engine
