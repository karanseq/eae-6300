#include "SmartMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "MonsterChase.h"

inline SmartMonsterController& SmartMonsterController::operator=(const SmartMonsterController& controller)
{
	if (this != &controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(controller.game_object_->GetTransform());
		target_ = controller.target_;
	}
	return *this;
}

inline SmartMonsterController& SmartMonsterController::operator=(SmartMonsterController&& controller)
{
	if (this != &controller)
	{
		std::swap(game_object_, controller.game_object_);
		target_ = controller.target_;
	}
	return *this;
}

inline engine::gameobject::GameObject* SmartMonsterController::GetGameObject()
{ 
	return game_object_;
}

inline void SmartMonsterController::SetGameObject(engine::gameobject::GameObject* game_object)
{ 
	ASSERT(game_object);
	SAFE_DELETE(game_object_);
	game_object_ = game_object;
}

inline engine::gameobject::GameObject* SmartMonsterController::GetTarget()
{
	return target_;
}

inline void SmartMonsterController::SetTarget(engine::gameobject::GameObject* target) 
{ 
	ASSERT(target);
	SAFE_DELETE(target_);
	target_ = target;
}