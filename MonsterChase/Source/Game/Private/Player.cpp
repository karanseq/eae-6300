#include "Game\Player.h"

// engine includes
#include "GLib.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorOverrides.h"
#include "Physics\PhysicsObject.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\MonsterChase.h"

namespace monsterchase {

Player::Player(const char* i_name) : controller_(new (MonsterChase::GetAllocator()) PlayerController()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(0, 0, i_name)),
	sprite_(GameUtils::CreateSprite(GameData::PLAYER_TEXTURE_NAME))
{
	(static_cast<PlayerController*>(controller_))->GetPhysicsObject()->SetMass(50.0f);
}

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
	PlayerController* player_controller = static_cast<PlayerController*>(controller_);
	if (MonsterChase::KEY_A_PRESSED)
	{
		player_controller->Move(MoveDirections::kMoveDirectionLeft);
	}
	if (MonsterChase::KEY_D_PRESSED)
	{
		player_controller->Move(MoveDirections::kMoveDirectionRight);
	}
	if (MonsterChase::KEY_W_PRESSED)
	{
		player_controller->Move(MoveDirections::kMoveDirectionUp);
	}
	if (MonsterChase::KEY_S_PRESSED)
	{
		player_controller->Move(MoveDirections::kMoveDirectionDown);
	}

	controller_->UpdateGameObject();
}

void Player::Render()
{
	if (sprite_)
	{
		GLib::Point2D offset = { controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y() };
		GLib::Sprites::RenderSprite(*sprite_, offset, 0.0f);
	}
}

void Player::Print()
{
	VERBOSE("Player %s is at [%f, %f]", identity_->GetName(), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}

} // namespace monsterchase