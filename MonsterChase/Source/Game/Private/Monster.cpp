#include "Game\Monster.h"

// library includes
#include <stdio.h>

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

Monster::Monster(MonsterControllers controller_type) : controller_(nullptr),
	identity_(new (MonsterChase::GetAllocator()) engine::IdentityComponent()),
	time_to_live_(0)
{
	ASSERT(controller_type != MonsterControllers::kNoMonsterController);
	switch (controller_type)
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

void Monster::Update()
{
	controller_->UpdateGameObject();
	--time_to_live_;
}

void Monster::Print()
{
	printf("Monster %s of type %d, %s [%f, %f]\n", identity_->GetName(), identity_->GetTag(), (time_to_live_ <= 1 ? "is about to die at" : "is at"), controller_->GetGameObject()->GetPosition().x(), controller_->GetGameObject()->GetPosition().y());
}