#include "Game\Asteroid.h"

// engine includes
#include "Assert\Assert.h"
#include "Time\Updater.h"

// game includes
#include "Game\Game.h"

namespace game {
    
Asteroid::Asteroid(const engine::memory::SharedPointer<engine::gameobject::Actor>& i_actor) : actor_(i_actor),
    force_(engine::math::Vec3D::ZERO)
{
    ASSERT(actor_);

    float foo = 0.5f + float(rand() % 50) * 0.01f;
    force_.x(foo * (rand() % 10 > 5 ? 1.0f : -1.0f));
    foo = 0.5f + float(rand() % 50) * 0.01f;
    force_.y(foo * (rand() % 10 > 5 ? 1.0f : -1.0f));

    /*auto physics_object = actor_->GetPhysicsObject().Lock();
    physics_object->SetDrag(physics_object->GetDrag() * 7.5f);*/

    // register for update events
    engine::time::Updater::Get()->AddTickable(this);
}

Asteroid::~Asteroid()
{
    // deregister for update events
    engine::time::Updater::Get()->RemoveTickable(this);
}

void Asteroid::Update(float i_dt)
{
    actor_->GetPhysicsObject().Lock()->ApplyImpulse(force_);

    // wrap around the screen
    engine::math::Vec3D position = actor_->GetGameObject()->GetPosition();
    position.x((position.x() < -Game::SCREEN_WIDTH / 2) ? Game::SCREEN_WIDTH / 2 : (position.x() > Game::SCREEN_WIDTH / 2 ? -Game::SCREEN_WIDTH / 2 : position.x()));
    position.y((position.y() < -Game::SCREEN_HEIGHT / 2) ? Game::SCREEN_HEIGHT / 2 : (position.y() > Game::SCREEN_HEIGHT / 2 ? -Game::SCREEN_HEIGHT / 2 : position.y()));
    actor_->GetGameObject()->SetPosition(position);
}

} // namespace game

