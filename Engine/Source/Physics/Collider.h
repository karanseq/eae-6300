#ifndef COLLIDER_H_
#define COLLIDER_H_

// library includes
#include <mutex>
#include <vector>

// engine includes
#include "Memory\WeakPointer.h"

// forward declarations
namespace engine {
namespace math {
    struct AABB;
    class Mat44;
    class Transform;
    class Vec3D;
}
namespace physics {
    class PhysicsObject;
}
}

namespace engine {
namespace physics {

class Collider
{
private:
    Collider();
    ~Collider();
    static Collider* instance_;

    // disable copy constructor & copy assignment operator
    Collider(const Collider& i_copy) = delete;
    Collider& operator=(const Collider& i_other) = delete;

public:
    static Collider* Create();
    static void Destroy();
    static inline Collider* Get();

    void Run(float i_dt);

    // add and remove physics objects
    void AddPhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);
    void RemovePhysicsObject(const engine::memory::WeakPointer<engine::physics::PhysicsObject>& i_physics_object);

#ifdef BUILD_DEBUG
    void PrintDebugInformation(const engine::math::Mat44& i_mat_WtoA,
        const engine::math::Mat44& i_mat_WtoB,
        const engine::math::Mat44& i_mat_AtoB, 
        const engine::math::Mat44& i_mat_BtoA, 
        const engine::math::AABB& i_a_aabb, 
        const engine::math::AABB& i_b_aabb, 
        const engine::math::Vec3D& i_a_vel, 
        const engine::math::Vec3D& i_b_vel,
        float i_dt) const;
#endif

private:
    size_t                                                                          num_physics_objects_;
    std::vector<engine::memory::WeakPointer<PhysicsObject>>                         physics_objects_;
    std::mutex                                                                      collider_mutex_;

}; // class Collider

} // namespace physics
} // namespace engine

#include "Collider-inl.h"

#endif // COLLIDER_H_
