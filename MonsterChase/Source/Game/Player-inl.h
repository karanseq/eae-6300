#include "Player.h"

// engine includes
#include "GLib.h"

// game includes
#include "Game\GameUtils.h"

namespace monsterchase {

inline Player& Player::operator=(const Player& i_player)
{
	if (this != &i_player)
	{
		SAFE_DELETE(controller_);
		controller_ = i_player.controller_->Clone();

		SAFE_DELETE(identity_);
		identity_ = new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(i_player.identity_->GetID(), i_player.identity_->GetTag(), i_player.identity_->GetName());

		if (sprite_)
		{
			GLib::Sprites::Release(sprite_);
		}
		sprite_ = GameUtils::CreateSprite(Player::texture_name_);
	}
	return *this;
}

inline Player& Player::operator=(Player&& i_player)
{
	if (this != &i_player)
	{
		std::swap(controller_, i_player.controller_);
		std::swap(identity_, i_player.identity_);
		std::swap(sprite_, i_player.sprite_);
	}
	return *this;
}

inline engine::gameobject::InterfaceGameObjectController* Player::GetController() const
{
	return controller_;
}

inline void Player::SetController(engine::gameobject::InterfaceGameObjectController* i_controller)
{
	ASSERT(i_controller);
	SAFE_DELETE(controller_);
	controller_ = i_controller;
}

inline engine::gameobject::IdentityComponent* Player::GetIdentity() const
{
	return identity_;
}

inline void Player::SetIdentity(engine::gameobject::IdentityComponent* i_identity)
{
	ASSERT(i_identity);
	SAFE_DELETE(identity_);
	identity_ = i_identity;
}

} // namespace monsterchase
