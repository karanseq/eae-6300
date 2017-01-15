#ifndef MONSTER_H_
#define MONSTER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\IdentityComponent.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\GameTypes.h"
#include "Game\SillyMonsterController.h"
#include "Game\SmartMonsterController.h"

// forward declarations
namespace GLib {
namespace Sprites {
	struct Sprite;
}
}

namespace monsterchase {

class Monster
{
public:
	Monster(MonsterControllers i_controller_type, const char* i_name);
	~Monster();

	// copy constructor
	Monster(const Monster& i_copy);
	// move constructor
	Monster(Monster&& i_copy);

	// copy assignment operator
	inline Monster& operator=(const Monster& i_monster);
	// move assignment operator
	inline Monster& operator=(Monster&& i_monster);

	void Update();
	void Render();
	void Print();

	// accessors and mutators
	inline engine::gameobject::InterfaceGameObjectController* GetController() const;
	inline void SetController(engine::gameobject::InterfaceGameObjectController* i_controller);

	inline engine::gameobject::IdentityComponent* GetIdentity() const;
	inline void SetIdentity(engine::gameobject::IdentityComponent* i_identity);

	inline uint8_t GetTimeToLive() const;
	inline void SetTimeToLive(uint8_t i_time_to_live);

private:
	engine::gameobject::InterfaceGameObjectController*			controller_;
	engine::gameobject::IdentityComponent*						identity_;
	GLib::Sprites::Sprite*										sprite_;
	MonsterControllers											controller_type_;
	uint8_t														time_to_live_;

	static const char*											silly_monster_texture_name_;
	static const char*											smart_monster_texture_name_;
}; // class Monster

} // namespace monsterchase

#include "Game\Monster-inl.h"

#endif // MONSTER_H_