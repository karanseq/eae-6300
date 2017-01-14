#include "Game\Monster.h"

// library includes
#include <stdio.h>

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

namespace monsterchase {

Monster::Monster(MonsterControllers i_controller_type, const char* i_name) : controller_(nullptr),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(0, 0, i_name)),
	time_to_live_(0)
{
	ASSERT(i_controller_type != MonsterControllers::kNoMonsterController);
	switch (i_controller_type)
	{
	case MonsterControllers::kSmartMonsterController:
		controller_ = new (MonsterChase::GetAllocator()) SmartMonsterController();
		identity_->SetTag(static_cast<uint32_t>(MonsterControllers::kSmartMonsterController));
		break;

	case MonsterControllers::kSillyMonsterController:
		controller_ = new (MonsterChase::GetAllocator()) SillyMonsterController();
		identity_->SetTag(static_cast<uint32_t>(MonsterControllers::kSillyMonsterController));
		break;
	}
}

Monster::~Monster()
{
	SAFE_DELETE(controller_);
	SAFE_DELETE(identity_);
}

Monster::Monster(const Monster& i_copy) : controller_(i_copy.controller_->Clone()),
	identity_(new (MonsterChase::GetAllocator()) engine::gameobject::IdentityComponent(i_copy.identity_->GetID(), i_copy.identity_->GetTag(), i_copy.identity_->GetName())),
	time_to_live_(i_copy.time_to_live_)
{}

Monster::Monster(Monster&& i_copy) : controller_(i_copy.controller_),
	identity_(i_copy.identity_),
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

void Monster::Print()
{
	printf("Monster %s of type %d, %s [%f, %f]\n", identity_->GetName(), identity_->GetTag(), (time_to_live_ <= 1 ? "is about to die at" : "is at"), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}

} // namespace monsterchase