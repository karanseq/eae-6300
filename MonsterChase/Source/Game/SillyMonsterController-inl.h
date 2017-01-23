#include "SillyMonsterController.h"

// engine includes
#include "Common\HelperMacros.h"

// game includes
#include "MonsterChase.h"

namespace monsterchase {

inline SillyMonsterController& SillyMonsterController::operator=(const SillyMonsterController& i_controller)
{
	if (this != &i_controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(i_controller.game_object_->GetTransform());
	}
	return *this;
}

inline SillyMonsterController& SillyMonsterController::operator=(SillyMonsterController&& i_controller)
{
	if (this != &i_controller)
	{
		std::swap(game_object_, i_controller.game_object_);
	}
	return *this;
}

inline engine::gameobject::GameObject* SillyMonsterController::GetGameObject()
{ 
	return game_object_;
}

inline void SillyMonsterController::SetGameObject(engine::gameobject::GameObject* i_game_object) 
{ 
	ASSERT(i_game_object);
	SAFE_DELETE(game_object_);
	game_object_ = i_game_object;
}

} // namespace monsterchase