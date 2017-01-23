#include "SmartMonsterController.h"

// engine includes
#include "Common\HelperMacros.h"

// game includes
#include "MonsterChase.h"

namespace monsterchase {

inline SmartMonsterController& SmartMonsterController::operator=(const SmartMonsterController& i_controller)
{
	if (this != &i_controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(i_controller.game_object_->GetTransform());
		target_ = i_controller.target_;
	}
	return *this;
}

inline SmartMonsterController& SmartMonsterController::operator=(SmartMonsterController&& i_controller)
{
	if (this != &i_controller)
	{
		std::swap(game_object_, i_controller.game_object_);
		target_ = i_controller.target_;
	}
	return *this;
}

inline engine::gameobject::GameObject* SmartMonsterController::GetGameObject()
{ 
	return game_object_;
}

inline void SmartMonsterController::SetGameObject(engine::gameobject::GameObject* i_game_object)
{ 
	ASSERT(i_game_object);
	SAFE_DELETE(game_object_);
	game_object_ = i_game_object;
}

inline engine::gameobject::GameObject* SmartMonsterController::GetTarget()
{
	return target_;
}

inline void SmartMonsterController::SetTarget(engine::gameobject::GameObject* i_target) 
{ 
	ASSERT(i_target);
	SAFE_DELETE(target_);
	target_ = i_target;
}

} // namespace monsterchase