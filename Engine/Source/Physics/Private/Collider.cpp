#include "Physics\Collider.h"

// engine includes
#include "Common\HelperMacros.h"
#include "GameObject\GameObject.h"
#include "Math\Mat44.h"
#include "Physics\PhysicsObject.h"

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
    for (size_t i = 0; i < num_physics_objects_; ++i)
    {
        // get physics object A
        const engine::memory::SharedPointer<PhysicsObject> physics_object_a = physics_objects_[i].Lock();

        // get game object A
        const engine::memory::SharedPointer<engine::gameobject::GameObject> game_object_a = physics_object_a->GetGameObject().Lock();
        // get A's AABB
        const engine::math::AABB a_aabb = game_object_a->GetAABB();

        // calculate transform to convert from object A to world coordinates
        engine::math::Mat44 mat_AtoW;
        engine::math::GetObjectToWorldTransform(game_object_a->GetTransform(), mat_AtoW);

        // calculate transform to convert world to object A coordinates
        engine::math::Mat44 mat_WtoA(mat_AtoW.GetInverse());

        for (size_t j = 0; j < num_physics_objects_; ++j)
        {
            // don't compute collisions with self
            if (physics_objects_[i] == physics_objects_[j])
            {
                continue;
            }

            // get physics object B
            engine::memory::SharedPointer<PhysicsObject> physics_object_b = physics_objects_[j].Lock();
            // get game object B
            const engine::memory::SharedPointer<engine::gameobject::GameObject> game_object_b = physics_object_b->GetGameObject().Lock();
            // get B's AABB
            const engine::math::AABB b_aabb = game_object_b->GetAABB();

            // calculate transform to convert object B to world coordinates
            engine::math::Mat44 mat_BtoW;
            engine::math::GetObjectToWorldTransform(game_object_b->GetTransform(), mat_BtoW);

            // calculate transform to convert world to object B coordinates
            engine::math::Mat44 mat_WtoB(mat_BtoW.GetInverse());

            // calculate transform to convert from A to B coordinates
            engine::math::Mat44 mat_AtoB = mat_WtoB * mat_AtoW;
            // calculate transform to convert from B to A coordinates
            engine::math::Mat44 mat_BtoA(mat_AtoB.GetInverse());

            bool is_X_separated_in_B = false;
            bool is_Y_separated_in_B = false;

            float t_close_X_in_B = 0.0f;
            float t_open_X_in_B = 0.0f;
            float t_close_Y_in_B = 0.0f;
            float t_open_Y_in_B = 0.0f;

            // check for A in B's coordinate system
            {
                // calculate velocity of A relative to B
                const engine::math::Vec3D relative_vel_AtoB = physics_object_a->GetVelocity() - physics_object_b->GetVelocity();

                // transform relative velocity (as a vector) to object B's coordinate system
                const engine::math::Vec4D relative_vel_WtoB = mat_WtoB * engine::math::Vec4D(relative_vel_AtoB, 0.0f);

                // transform A's AABB to B's coordinate system
                const engine::math::Vec4D A_center_in_B = mat_AtoB * engine::math::Vec4D(a_aabb.center, 1.0f);
                const engine::math::Vec4D A_X_extent_in_B = mat_AtoB * engine::math::Vec4D(a_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
                const engine::math::Vec4D A_Y_extent_in_B = mat_AtoB * engine::math::Vec4D(0.0f, a_aabb.extents.y(), 0.0f, 0.0f);
                const engine::math::Vec4D A_extents_in_B(fabs(A_X_extent_in_B.x()) + fabs(A_Y_extent_in_B.x()), fabs(A_X_extent_in_B.y()) + fabs(A_Y_extent_in_B.y()), 0.0f, 0.0f);

                // for X- axis
                {
                    // treat zero velocities differently
                    if (engine::math::FuzzyEquals(relative_vel_WtoB.x(), 0.0f))
                    {
                        // separation check without velocities
                        //is_X_separated_in_B = fabs(A_center_in_B.x() - b_aabb.center.x()) > A_X_extent_in_B.x() + b_aabb.extents.x();
                        is_X_separated_in_B = fabs(A_center_in_B.x() - b_aabb.center.x()) > A_extents_in_B.x() + b_aabb.extents.x();
                    }
                    else
                    {
                        // calculate separation close and open times
                        t_close_X_in_B = (b_aabb.center.x() - b_aabb.extents.x() - A_center_in_B.x() - A_extents_in_B.x()) / relative_vel_WtoB.x();
                        t_open_X_in_B = (b_aabb.center.x() + b_aabb.extents.x() - A_center_in_B.x() + A_extents_in_B.x()) / relative_vel_WtoB.x();

                        // t_close must be less than t_open
                        // if not, swap them
                        if (t_open_X_in_B < t_close_X_in_B)
                        {
                            float t_swap = t_open_X_in_B;
                            t_open_X_in_B = t_close_X_in_B;
                            t_close_X_in_B = t_swap;
                        }

                        // if t_open < 0, the separation occurred in the past
                        // if t_close > i_dt, the separation will occur in the future
                        is_X_separated_in_B = t_open_X_in_B < 0 || t_close_X_in_B > i_dt;
                    }

                } // for X- axis

                // for Y-axis...only if no separation was found on the X-axis
                if (!is_X_separated_in_B)
                {
                    // treat zero velocities differently
                    if (engine::math::FuzzyEquals(relative_vel_WtoB.y(), 0.0f))
                    {
                        // separation check without velocities
                        is_Y_separated_in_B = fabs(A_center_in_B.y() - b_aabb.center.y()) > A_extents_in_B.y() + b_aabb.extents.y();
                    }
                    else
                    {
                        // calculate separation close and open times
                        t_close_Y_in_B = (b_aabb.center.y() - b_aabb.extents.y() - A_center_in_B.y() - A_extents_in_B.y()) / relative_vel_WtoB.y();
                        t_open_Y_in_B = (b_aabb.center.y() + b_aabb.extents.y() - A_center_in_B.y() + A_extents_in_B.y()) / relative_vel_WtoB.y();

                        // t_close must be less than t_open
                        // if not, swap them
                        if (t_open_Y_in_B < t_close_Y_in_B)
                        {
                            float t_swap = t_open_Y_in_B;
                            t_open_Y_in_B = t_close_Y_in_B;
                            t_close_Y_in_B = t_swap;
                        }

                        // if t_open < 0, the separation occurred in the past
                        // if t_close > i_dt, the separation will occur in the future
                        is_Y_separated_in_B = t_open_Y_in_B < 0 || t_close_Y_in_B > i_dt;
                    }

                } // for Y-axis

            } // check for A in B's coordinate system    

            bool is_X_separated_in_A = false;
            bool is_Y_separated_in_A = false;

            float t_close_X_in_A = 0.0f;
            float t_open_X_in_A = 0.0f;
            float t_close_Y_in_A = 0.0f;
            float t_open_Y_in_A = 0.0f;

            // check for B in A's coordinate system
            {
                // calculate velocity of B relative to A
                const engine::math::Vec3D relative_vel_BtoA = physics_object_b->GetVelocity() - physics_object_a->GetVelocity();

                // transform relative velocity (as a vector) to A's coordinate system
                const engine::math::Vec4D relative_vel_WtoA = mat_WtoA * engine::math::Vec4D(relative_vel_BtoA, 0.0f);

                // transform B's AABB to A's coordinate system
                const engine::math::Vec4D B_center_in_A = mat_BtoA * engine::math::Vec4D(b_aabb.center, 1.0f);
                const engine::math::Vec4D B_X_extent_in_A = mat_BtoA * engine::math::Vec4D(b_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
                const engine::math::Vec4D B_Y_extent_in_A = mat_BtoA * engine::math::Vec4D(0.0f, b_aabb.extents.y(), 0.0f, 0.0f);
                const engine::math::Vec4D B_extents_in_A(fabs(B_X_extent_in_A.x()) + fabs(B_Y_extent_in_A.x()), fabs(B_X_extent_in_A.y()) + fabs(B_Y_extent_in_A.y()), 0.0f, 0.0f);

                // for X-axis
                {
                    // treat zero velocities differently
                    if (engine::math::FuzzyEquals(relative_vel_WtoA.x(), 0.0f))
                    {
                        // separation check without velocities
                        is_X_separated_in_A = fabs(a_aabb.center.x() - B_center_in_A.x()) > a_aabb.extents.x() + B_extents_in_A.x();
                    }
                    else
                    {
                        // calculate separation close and open times
                        t_close_X_in_A = (a_aabb.center.x() - a_aabb.extents.x() - B_center_in_A.x() - B_extents_in_A.x()) / relative_vel_WtoA.x();
                        t_open_X_in_A = (a_aabb.center.x() + a_aabb.extents.x() - B_center_in_A.x() + B_extents_in_A.x()) / relative_vel_WtoA.x();

                        // t_close must be less than t_open
                        // if not, swap them
                        if (t_open_X_in_A < t_close_X_in_A)
                        {
                            float t_swap = t_open_X_in_A;
                            t_open_X_in_A = t_close_X_in_A;
                            t_close_X_in_A = t_swap;
                        }

                        // if t_open < 0, the separation occurred in the past
                        // if t_close > i_dt, the separation will occur in the future
                        is_X_separated_in_A = t_open_X_in_A < 0 || t_close_X_in_A > i_dt;
                    }
                }

                // for Y-axis...only if no separation was found on the X-axis
                if (!is_X_separated_in_A)
                {
                    // treat zero velocities differently
                    if (engine::math::FuzzyEquals(relative_vel_WtoA.y(), 0.0f))
                    {
                        // separation check without velocities
                        is_Y_separated_in_A = fabs(a_aabb.center.y() - B_center_in_A.y()) > a_aabb.extents.y() + B_extents_in_A.y();
                    }
                    else
                    {
                        // calculate separation close and open times
                        t_close_Y_in_A = (a_aabb.center.y() - a_aabb.extents.y() - B_center_in_A.y() - B_extents_in_A.y()) / relative_vel_WtoA.y();
                        t_open_Y_in_A = (a_aabb.center.y() + a_aabb.extents.y() - B_center_in_A.y() + B_extents_in_A.y()) / relative_vel_WtoA.y();

                        // t_close must be less than t_open
                        // if not, swap them
                        if (t_open_Y_in_A < t_close_Y_in_A)
                        {
                            float t_swap = t_open_Y_in_A;
                            t_open_Y_in_A = t_close_Y_in_A;
                            t_close_Y_in_A = t_swap;
                        }

                        // if t_open < 0, the separation occurred in the past
                        // if t_close > i_dt, the separation will occur in the future
                        is_Y_separated_in_A = t_open_Y_in_A < 0 || t_close_Y_in_A > i_dt;
                    }

                }

            } // check for B in A's coordinate system

            // was there an obvious separation?
            if (!(is_X_separated_in_B || is_Y_separated_in_B || is_X_separated_in_A || is_Y_separated_in_A))
            {
                // find the latest t_close and the earliest t_open
                float t_close_latest = engine::math::GetMaxOfFour(t_close_X_in_B, t_close_Y_in_B, t_close_X_in_A, t_close_Y_in_A);
                float t_open_earliest = engine::math::GetMinOfFour(t_open_X_in_B, t_open_Y_in_B, t_open_X_in_A, t_open_Y_in_A);

                // if the latest t_close was after the earliest t_open, there was continuity of separation
                if (t_close_latest > t_open_earliest)
                {
                    VERBOSE("Collision not found!");
                }
                else
                {
                    VERBOSE("Collision found!");
#ifdef BUILD_DEBUG
                    /*PrintDebugInformation(mat_WtoA,
                        mat_WtoB,
                        mat_AtoB,
                        mat_BtoA,
                        game_object_a->GetAABB(),
                        game_object_b->GetAABB(),
                        physics_object_a->GetVelocity(),
                        physics_object_b->GetVelocity(),
                        i_dt);*/
#endif

                    physics_object_a->ApplyImpulse(physics_object_a->GetVelocity() * -5.0f);
                    physics_object_b->ApplyImpulse(physics_object_b->GetVelocity() * -5.0f);
                }
            }
            else
            {
                VERBOSE("Collision not found!");
            }

        } // end of inner for loop

    } // end of outer for loop

}

#ifdef BUILD_DEBUG
void Collider::PrintDebugInformation(const engine::math::Mat44& i_mat_WtoA,
    const engine::math::Mat44& i_mat_WtoB, 
    const engine::math::Mat44& i_mat_AtoB,
    const engine::math::Mat44& i_mat_BtoA, 
    const engine::math::AABB& i_a_aabb, 
    const engine::math::AABB& i_b_aabb, 
    const engine::math::Vec3D& i_a_vel, 
    const engine::math::Vec3D& i_b_vel,
    float i_dt) const
{
    LOG("******************************");

    const engine::math::Vec4D A_center_in_B = i_mat_AtoB * engine::math::Vec4D(i_a_aabb.center, 1.0f);
    const engine::math::Vec4D A_X_extent_in_B = i_mat_AtoB * engine::math::Vec4D(i_a_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
    const engine::math::Vec4D A_Y_extent_in_B = i_mat_AtoB * engine::math::Vec4D(0.0f, i_a_aabb.extents.y(), 0.0f, 0.0f);
    const engine::math::Vec4D A_extents_in_B = A_X_extent_in_B + A_Y_extent_in_B;

    const engine::math::Vec4D B_center_in_A = i_mat_BtoA * engine::math::Vec4D(i_b_aabb.center, 1.0f);
    const engine::math::Vec4D B_X_extent_in_A = i_mat_BtoA * engine::math::Vec4D(i_b_aabb.extents.x(), 0.0f, 0.0f, 0.0f);
    const engine::math::Vec4D B_Y_extent_in_A = i_mat_BtoA * engine::math::Vec4D(0.0f, i_b_aabb.extents.y(), 0.0f, 0.0f);
    const engine::math::Vec4D B_extents_in_A = B_X_extent_in_A + B_Y_extent_in_A;

    const engine::math::Vec4D relative_vel_in_A = i_mat_WtoA * engine::math::Vec4D(i_a_vel - i_b_vel, 0.0f);
    const engine::math::Vec4D relative_vel_in_B = i_mat_WtoB * engine::math::Vec4D(i_b_vel - i_a_vel, 0.0f);

    LOG("A_to_B:");
    i_mat_AtoB.Print();
    LOG("B_to_A:");
    i_mat_BtoA.Print();

    LOG("A_center: %f, %f", i_a_aabb.center.x(), i_a_aabb.center.y());
    LOG("A_center_in_B: %f, %f", A_center_in_B.x(), A_center_in_B.y());
    LOG("A_extents: %f, %f", i_a_aabb.extents.x(), i_a_aabb.extents.y());
    LOG("A_extents_in_B: %f, %f", A_extents_in_B.x(), A_extents_in_B.y());

    LOG("B_center: %f, %f", i_b_aabb.center.x(), i_b_aabb.center.y());
    LOG("B_center_in_A: %f, %f", B_center_in_A.x(), B_center_in_A.y());
    LOG("B_extents: %f, %f", i_b_aabb.extents.x(), i_b_aabb.extents.y());
    LOG("B_extents_in_A: %f, %f", B_extents_in_A.x(), B_extents_in_A.y());

    //LOG("X_separation_in_A:%f", fabs(i_a_aabb.center.x() - i_a_aabb.extents.x() - B_center_in_A.x() - B_X_extent_in_A.x()));
    //LOG("Y_separation_in_A:%f", fabs(i_a_aabb.center.y() - i_a_aabb.extents.y() - B_center_in_A.y() - B_Y_extent_in_A.y()));
    //LOG("X_separation_in_B:%f", fabs(i_b_aabb.center.x() - i_b_aabb.extents.x() - A_center_in_B.x() - A_X_extent_in_B.x()));
    //LOG("Y_separation_in_B:%f", fabs(i_b_aabb.center.y() - i_b_aabb.extents.y() - A_center_in_B.y() - A_Y_extent_in_B.y()));

    //LOG("t_open_X_in_A: %f", (i_a_aabb.center.x() + i_a_aabb.extents.x() - B_center_in_A.x() + B_X_extent_in_A.x()) / relative_vel_in_A.x());
    //LOG("t_close_X_in_A: %f", (i_a_aabb.center.x() - i_a_aabb.extents.x() - B_center_in_A.x() - B_X_extent_in_A.x()) / relative_vel_in_A.x());

    //LOG("t_open_Y_in_A: %f", (i_a_aabb.center.y() + i_a_aabb.extents.y() - B_center_in_A.y() + B_Y_extent_in_A.y()) / relative_vel_in_A.y());
    //LOG("t_close_Y_in_A: %f", (i_a_aabb.center.y() - i_a_aabb.extents.y() - B_center_in_A.y() - B_Y_extent_in_A.y()) / relative_vel_in_A.y());

    //LOG("t_open_X_in_B: %f", (i_b_aabb.center.x() + i_b_aabb.extents.x() - A_center_in_B.x() + A_X_extent_in_B.x()) / relative_vel_in_B.x());
    //LOG("t_close_X_in_B: %f", (i_b_aabb.center.x() - i_b_aabb.extents.x() - A_center_in_B.x() - A_X_extent_in_B.x()) / relative_vel_in_B.x());

    //LOG("t_open_Y_in_B: %f", (i_b_aabb.center.y() + i_b_aabb.extents.y() - A_center_in_B.y() + A_Y_extent_in_B.y()) / relative_vel_in_B.y());
    //LOG("t_close_Y_in_B: %f", (i_b_aabb.center.y() - i_b_aabb.extents.y() - A_center_in_B.y() - A_Y_extent_in_B.y()) / relative_vel_in_B.y());

    //LOG("relative_velocity_in_A: %f, %f", relative_vel_in_A.x(), relative_vel_in_A.y());
    //LOG("relative_velocity_in_B: %f, %f", relative_vel_in_B.x(), relative_vel_in_B.y());
    //LOG("DT:%f", i_dt);

    LOG("******************************\n");
}
#endif // BUILD_DEBUG

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