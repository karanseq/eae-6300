#include "Monster.h"

inline Monster& Monster::operator=(const Monster& monster)
{
	if (this != &monster)
	{
		SAFE_DELETE(controller_);
		controller_ = monster.controller_->Clone();

		SAFE_DELETE(identity_);
		identity_ = new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(monster.identity_->GetID(), monster.identity_->GetTag(), monster.identity_->GetName());

		time_to_live_ = monster.time_to_live_;
	}
	return *this;
}

inline Monster& Monster::operator=(Monster&& monster)
{
	if (this != &monster)
	{
		std::swap(controller_, monster.controller_);
		std::swap(identity_, monster.identity_);
		time_to_live_ = monster.time_to_live_;
	}
	return *this;
}

inline engine::gameobject::InterfaceGameObjectController* Monster::GetController() const
{
	return controller_;
}

inline void Monster::SetController(engine::gameobject::InterfaceGameObjectController* controller)
{
	ASSERT(controller);
	SAFE_DELETE(controller_);
	controller_ = controller;
}

inline engine::gameobject::IdentityComponent* Monster::GetIdentity() const
{
	return identity_;
}

inline void Monster::SetIdentity(engine::gameobject::IdentityComponent* identity)
{
	ASSERT(identity);
	SAFE_DELETE(identity_);
	identity_ = identity;
}

inline uint8_t Monster::GetTimeToLive() const
{
	return time_to_live_;
}

inline void Monster::SetTimeToLive(uint8_t time_to_live)
{
	time_to_live_ = time_to_live;
}