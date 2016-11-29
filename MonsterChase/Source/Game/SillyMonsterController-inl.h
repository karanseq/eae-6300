#include "SillyMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "MonsterChase.h"

inline SillyMonsterController& SillyMonsterController::operator=(const SillyMonsterController& controller)
{
	if (this != &controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(controller.game_object_->GetTransform());
	}
	return *this;
}

inline SillyMonsterController& SillyMonsterController::operator=(SillyMonsterController&& controller)
{
	if (this != &controller)
	{
		std::swap(game_object_, controller.game_object_);
	}
	return *this;
}

inline engine::gameobject::GameObject* SillyMonsterController::GetGameObject()
{ 
	return game_object_;
}

inline void SillyMonsterController::SetGameObject(engine::gameobject::GameObject* game_object) 
{ 
	ASSERT(game_object);
	SAFE_DELETE(game_object_);
	game_object_ = game_object;
}