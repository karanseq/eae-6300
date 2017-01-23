#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\IdentityComponent.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\PlayerController.h"

// forward declarations
namespace GLib {
namespace Sprites {
	struct Sprite;
}
}

namespace monsterchase {

class Player
{
public:
	Player(const char* i_name);
	~Player();

	// copy constructor
	Player(const Player& i_copy);
	// move constructor
	Player(Player&& i_copy);

	// copy assignment operator
	inline Player& operator=(const Player& i_player);
	// move assignment operator
	inline Player& operator=(Player&& i_player);

	void Update();
	void Render();
	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const;
	inline void SetController(engine::gameobject::InterfaceGameObjectController* i_controller);

	inline engine::gameobject::IdentityComponent* GetIdentity() const;
	inline void SetIdentity(engine::gameobject::IdentityComponent* i_identity);

	inline GLib::Sprites::Sprite* GetSprite() const;

private:
	engine::gameobject::InterfaceGameObjectController*					controller_;
	engine::gameobject::IdentityComponent*								identity_;
	GLib::Sprites::Sprite*												sprite_;
}; // class Player

} // namespace monsterchase

#include "Game\Player-inl.h"

#endif // PLAYER_H_