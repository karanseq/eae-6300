#include "Game\Player.h"

// library includes
#include <stdio.h>

// engine includes
#include "GLib.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\MonsterChase.h"

namespace monsterchase {

Player::Player(const char* i_name) : controller_(new (MonsterChase::GetAllocator()) PlayerController()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(0, 0, i_name)),
	sprite_(GameUtils::CreateSprite(GameData::PLAYER_TEXTURE_NAME))
{}

Player::~Player()
{
	SAFE_DELETE(controller_);
	SAFE_DELETE(identity_);
	if (sprite_)
	{
		GLib::Sprites::Release(sprite_);
	}
}

Player::Player(const Player& i_copy) : controller_(i_copy.controller_->Clone()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(i_copy.identity_->GetID(), i_copy.identity_->GetTag(), i_copy.identity_->GetName())),
	sprite_(GameUtils::CreateSprite(GameData::PLAYER_TEXTURE_NAME))
{}

Player::Player(Player&& i_copy) : controller_(i_copy.controller_),
	identity_(i_copy.identity_),
	sprite_(i_copy.sprite_)
{
	i_copy.controller_ = nullptr;
	i_copy.identity_ = nullptr;
	i_copy.sprite_ = nullptr;
}

void Player::Update()
{
	controller_->UpdateGameObject();
}

void Player::Render()
{
	if (sprite_)
	{
		GLib::Point2D offset = { controller_->GetGameObject()->GetPosition().x() * MonsterChase::TILE_SIZE, controller_->GetGameObject()->GetPosition().y() * MonsterChase::TILE_SIZE };
		GLib::Sprites::RenderSprite(*sprite_, offset, 0.0f);
	}
}

bool Player::HandleUserInput(KeyboardKeys i_key)
{
	switch (i_key)
	{
	case KeyboardKeys::kA:
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionLeft);
		return true;
	case KeyboardKeys::kD:
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionRight);
		return true;
	case KeyboardKeys::kW:
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionUp);
		return true;
	case KeyboardKeys::kS:
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionDown);
		return true;
	default:
		reinterpret_cast<PlayerController*>(controller_)->SetMoveDirection(MoveDirections::kMoveDirectionNone);
		return false;
	}
}

void Player::Print()
{
	LOG("Player %s is at [%f, %f]", identity_->GetName(), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}

} // namespace monsterchase