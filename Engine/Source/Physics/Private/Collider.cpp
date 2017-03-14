#include "Physics\Collider.h"

// engine includes
#include "Common\HelperMacros.h"

namespace engine {
namespace physics {

// static member initialization
Collider* Collider::instance_ = nullptr;

Collider::Collider() : num_physics_objects_(0)
{}

Collider::~Collider()
{
    physics_objects_.clear();
    num_physics_objects_ = 0;
}

Collider* Collider::Create()
{
    if (!Collider::instance_)
    {
        Collider::instance_ = new Collider();
    }
    return Collider::instance_;
}

void Collider::Destroy()
{
    SAFE_DELETE(Collider::instance_);
}

void Collider::Run(float i_dt)
{
    /*for (size_t i = 0; i < num_physics_objects_; ++i)
    {
        // calculate object A to world
        // calculate world to object A
        for (size_t j = 0; j < num_physics_objects_; ++j)
        {
            // calculate object B to world
            // calculate world to object B
        }
    }*/
}

void Collider::AddPhysicsObject(const engine::memory::WeakPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);

    // acquire a lock
    std::lock_guard<std::mutex> lock(collider_mutex_);

    // check if this object already exists
    if (std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object) != physics_objects_.end())
    {
        LOG_ERROR("Collider is already tracking this physics object!");
        return;
    }
    
    // add it to the list
    physics_objects_.push_back(i_physics_object);
    ++num_physics_objects_;
}

void Collider::RemovePhysicsObject(const engine::memory::WeakPointer<PhysicsObject>& i_physics_object)
{
    // validate input
    ASSERT(i_physics_object);
    // can't remove an object if there are none
    ASSERT(num_physics_objects_ > 0);

    // acquire a lock
    std::lock_guard<std::mutex> lock(collider_mutex_);

    // check if this object exists
    auto it = std::find(physics_objects_.begin(), physics_objects_.end(), i_physics_object);
    if (it == physics_objects_.end())
    {
        LOG_ERROR("Physics could not find this physics object!");
        return;
    }

    // remove it from the list
    physics_objects_.erase(it);
    --num_physics_objects_;
}

} // namespace physics
} // namespace engine