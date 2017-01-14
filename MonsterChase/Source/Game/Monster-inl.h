#include "Monster.h"

namespace monsterchase {

inline Monster& Monster::operator=(const Monster& i_monster)
{
	if (this != &i_monster)
	{
		SAFE_DELETE(controller_);
		controller_ = i_monster.controller_->Clone();

		SAFE_DELETE(identity_);
		identity_ = new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(i_monster.identity_->GetID(), i_monster.identity_->GetTag(), i_monster.identity_->GetName());

		time_to_live_ = i_monster.time_to_live_;
	}
	return *this;
}

inline Monster& Monster::operator=(Monster&& i_monster)
{
	if (this != &i_monster)
	{
		std::swap(controller_, i_monster.controller_);
		std::swap(identity_, i_monster.identity_);
		time_to_live_ = i_monster.time_to_live_;
	}
	return *this;
}

inline engine::gameobject::InterfaceGameObjectController* Monster::GetController() const
{
	return controller_;
}

inline void Monster::SetController(engine::gameobject::InterfaceGameObjectController* i_controller)
{
	ASSERT(i_controller);
	SAFE_DELETE(controller_);
	controller_ = i_controller;
}

inline engine::gameobject::IdentityComponent* Monster::GetIdentity() const
{
	return identity_;
}

inline void Monster::SetIdentity(engine::gameobject::IdentityComponent* i_identity)
{
	ASSERT(i_identity);
	SAFE_DELETE(identity_);
	identity_ = i_identity;
}

inline uint8_t Monster::GetTimeToLive() const
{
	return time_to_live_;
}

inline void Monster::SetTimeToLive(uint8_t i_time_to_live)
{
	time_to_live_ = i_time_to_live;
}

} // namespace monsterchase