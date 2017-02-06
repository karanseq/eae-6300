#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "Memory\SharedPointer.h"

// forward declarations
namespace engine {
namespace gameobject {
	class Actor;
}
}

namespace monsterchase {
	
class Player
{
public:
	Player();
	~Player();

	// disable copy & move constructors & assignment operators
	Player(const Player& i_copy) = delete;
	Player& operator=(const Player& i_copy) = delete;
	Player(Player&& i_copy) = delete;
	Player& operator=(Player&& i_copy) = delete;

	// constants
	static const float																DEFAULT_MASS;

private:
	engine::memory::SharedPointer<engine::gameobject::Actor>						actor_;

}; // class Player

} // namespace monsterchase

#endif // PLAYER_H_
