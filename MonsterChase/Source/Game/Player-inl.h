#include "Player.h"

inline Player& Player::operator=(const Player& player)
{
	if (this != &player)
	{
		SAFE_DELETE(controller_);
		controller_ = player.controller_->Clone();

		SAFE_DELETE(identity_);
		identity_ = new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(player.identity_->GetID(), player.identity_->GetTag(), player.identity_->GetName());
	}
	return *this;
}

inline Player& Player::operator=(Player&& player)
{
	if (this != &player)
	{
		std::swap(controller_, player.controller_);
		std::swap(identity_, player.identity_);
	}
	return *this;
}

inline engine::gameobject::InterfaceGameObjectController* Player::GetController() const
{
	return controller_;
}

inline void Player::SetController(engine::gameobject::InterfaceGameObjectController* controller)
{
	ASSERT(controller);
	SAFE_DELETE(controller_);
	controller_ = controller;
}

inline engine::gameobject::IdentityComponent* Player::GetIdentity() const
{
	return identity_;
}

inline void Player::SetIdentity(engine::gameobject::IdentityComponent* identity)
{
	ASSERT(identity);
	SAFE_DELETE(identity_);
	identity_ = identity;
}
