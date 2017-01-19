#include "Game\Monster.h"

// engine includes
#include "GLib.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\GameUtils.h"
#include "Game\MonsterChase.h"

namespace monsterchase {

Monster::Monster(MonsterControllers i_controller_type, const char* i_name) : controller_(nullptr),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(0, 0, i_name)),
	sprite_(nullptr),
	controller_type_(i_controller_type),
	time_to_live_(0)
{
	ASSERT(controller_type_ != MonsterControllers::kNoMonsterController);
	switch (controller_type_)
	{
	case MonsterControllers::kSmartMonsterController:
		controller_ = new (MonsterChase::GetAllocator()) SmartMonsterController();
		identity_->SetTag(static_cast<uint32_t>(MonsterControllers::kSmartMonsterController));
		sprite_ = GameUtils::CreateSprite(GameData::SMART_MONSTER_TEXTURE_NAME);
		break;

	case MonsterControllers::kSillyMonsterController:
		controller_ = new (MonsterChase::GetAllocator()) SillyMonsterController();
		identity_->SetTag(static_cast<uint32_t>(MonsterControllers::kSillyMonsterController));
		sprite_ = GameUtils::CreateSprite(GameData::SILLY_MONSTER_TEXTURE_NAME);
		break;
	}
}

Monster::~Monster()
{
	SAFE_DELETE(controller_);
	SAFE_DELETE(identity_);
	if (sprite_)
	{
		GLib::Sprites::Release(sprite_);
	}
}

Monster::Monster(const Monster& i_copy) : controller_(i_copy.controller_->Clone()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(i_copy.identity_->GetID(), i_copy.identity_->GetTag(), i_copy.identity_->GetName())),
	sprite_(i_copy.controller_type_ == MonsterControllers::kSillyMonsterController ? GameUtils::CreateSprite(GameData::SILLY_MONSTER_TEXTURE_NAME) : (i_copy.controller_type_ == MonsterControllers::kSmartMonsterController ? GameUtils::CreateSprite(GameData::SMART_MONSTER_TEXTURE_NAME) : nullptr)),
	controller_type_(i_copy.controller_type_),
	time_to_live_(i_copy.time_to_live_)
{}

Monster::Monster(Monster&& i_copy) : controller_(i_copy.controller_),
	identity_(i_copy.identity_),
	sprite_(i_copy.sprite_),
	controller_type_(i_copy.controller_type_),
	time_to_live_(i_copy.time_to_live_)
{
	i_copy.controller_ = nullptr;
	i_copy.identity_ = nullptr;
}

void Monster::Update()
{
	controller_->UpdateGameObject();
	--time_to_live_;
}

void Monster::Render()
{
	if (sprite_)
	{
		GLib::Point2D offset = { controller_->GetGameObject()->GetPosition().x() * MonsterChase::TILE_SIZE, controller_->GetGameObject()->GetPosition().y() * MonsterChase::TILE_SIZE };
		GLib::Sprites::RenderSprite(*sprite_, offset, 0.0f);
	}
}

void Monster::Print()
{
	VERBOSE("Monster %s of type %d, %s [%f, %f]", identity_->GetName(), identity_->GetTag(), (time_to_live_ <= 1 ? "is about to die at" : "is at"), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}

} // namespace monsterchase