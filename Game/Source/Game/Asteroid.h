#ifndef ASTEROID_H_
#define ASTEROID_H_

// engine includes
#include "GameObject\Actor.h"
#include "Math\Vec3D.h"
#include "Memory\SharedPointer.h"
#include "Time\InterfaceTickable.h"

namespace game {

class Asteroid : public engine::time::InterfaceTickable
{
public:
    Asteroid(const engine::memory::SharedPointer<engine::gameobject::Actor>& i_actor);
    ~Asteroid();

    // disable copy & move constructors & assignment operators
    Asteroid(const Asteroid& i_copy) = delete;
    Asteroid& operator=(const Asteroid& i_copy) = delete;
    Asteroid(Asteroid&& i_copy) = delete;
    Asteroid& operator=(Asteroid&& i_copy) = delete;

    // implement interface tickable
    void Update(float i_dt) override;

    // accessors & mutators
    inline const engine::memory::WeakPointer<engine::gameobject::Actor> GetActor() { return actor_; }

    inline const engine::math::Vec3D& GetForce() const { return force_; }
    inline void SetForce(const engine::math::Vec3D& i_force) { force_ = i_force; }

private:
    engine::memory::SharedPointer<engine::gameobject::Actor>                    actor_;
    engine::math::Vec3D                                                         force_;

}; // class Asteroid

} // namespace game

#endif // ASTEROID_H_
