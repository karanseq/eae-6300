#ifndef GAME_H_
#define GAME_H_

// library includes
#include <mutex>
#include <stdint.h>
#include <vector>

// engine includes
#include "Events\KeyboardEvent.h"
#include "Events\TimerEvent.h"
#include "GameObject\Actor.h"
#include "Memory\SharedPointer.h"
#include "Memory\WeakPointer.h"
#include "Physics\Collider.h"
#include "Time\InterfaceTickable.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameData.h"
#include "Game\GameTypes.h"

namespace game {

// forward declarations
class Player;

// global life-cycle functions
bool StartUp();
void Shutdown();

class Game : public engine::time::InterfaceTickable, public engine::physics::InterfaceCollisionListener
{
public:
    static Game* Create();
    static inline Game* GetInstance()                                               { return instance_; }
    static void Destroy();

    // game life-cycle functions
    bool Init();
    void Reset();

    // asset loading event handlers
    void OnLoadingComplete();
    void OnLoadingFailed();

    // the main game loop
    // called every tick by the engine
    virtual void Tick(float dt) override;

    // gameplay
    void OnKeyPressed(unsigned int i_key_id);
    void CreatePlayer();
    void DestroyPlayer();
    void CreateActor(const engine::data::PooledString& i_file_name);

    // collision listener
    virtual void OnCollision(const engine::physics::CollisionPair& i_collision_pair) override;

    inline GameStates GetState() const                                              { return game_state_; }

    // game constants
    static const uint16_t                                                           SCREEN_WIDTH = 1280;
    static const uint16_t                                                           SCREEN_HEIGHT = 800;

private:
    Game();
    ~Game();
    static Game* instance_;

    // disable copy constructor & copy assignment operator
    Game(const Game& i_copy) = delete;
    Game& operator=(const Game& i_copy) = delete;

    void OnFileLoaded(const engine::util::FileUtils::FileData i_file_data) const;
    void OnActorCreated(engine::memory::SharedPointer<engine::gameobject::Actor>) const;

    // game state & data
    GameStates                                                                      game_state_;
    GameData                                                                        game_data_;
    bool                                                                            init_successful_;

    // game elements
    Player*                                                                         player_;
    std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>           actors_;

    // events
    engine::memory::SharedPointer<engine::events::KeyboardEvent>                    keyboard_event_;
    engine::memory::WeakPointer<engine::events::TimerEvent>                         move_enemies_event_;
    engine::memory::WeakPointer<engine::events::TimerEvent>                         fire_enemy_bullet_event_;

    mutable std::mutex                                                              new_actors_mutex_;
    mutable std::vector<engine::memory::SharedPointer<engine::gameobject::Actor>>   new_actors_;

}; // class Game

} // namespace game

#endif // GAME_H_