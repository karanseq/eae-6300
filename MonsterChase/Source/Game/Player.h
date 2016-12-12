#ifndef PLAYER_H_
#define PLAYER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\IdentityComponent.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\PlayerController.h"

class Player
{
public:
	Player(const char* name);
	~Player();

	// copy constructor
	Player(const Player& copy);
	// move constructor
	Player(Player&& copy);

	// copy assignment operator
	inline Player& operator=(const Player& player);
	// move assignment operator
	inline Player& operator=(Player&& player);

	void Update();
	bool HandleUserInput(char input);
	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const;
	inline void SetController(engine::gameobject::InterfaceGameObjectController* controller);

	inline engine::gameobject::IdentityComponent* GetIdentity() const;
	inline void SetIdentity(engine::gameobject::IdentityComponent* identity);

private:
	engine::gameobject::InterfaceGameObjectController*					controller_;
	engine::gameobject::IdentityComponent*								identity_;
}; // class Player

#include "Game\Player-inl.h"

#endif // PLAYER_H_
