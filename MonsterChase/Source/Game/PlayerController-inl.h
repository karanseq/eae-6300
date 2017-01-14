#include "PlayerController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "MonsterChase.h"

namespace monsterchase {

inline PlayerController& PlayerController::operator=(const PlayerController& i_controller)
{
	if (this != &i_controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(i_controller.game_object_->GetTransform());
		move_direction_ = i_controller.move_direction_;
	}
	return *this;
}

inline PlayerController& PlayerController::operator=(PlayerController&& i_controller)
{
	if (this != &i_controller)
	{
		std::swap(game_object_, i_controller.game_object_);
		move_direction_ = i_controller.move_direction_;
	}
	return *this;
}

inline engine::gameobject::GameObject* PlayerController::GetGameObject()
{
	return game_object_;
}

inline void PlayerController::SetGameObject(engine::gameobject::GameObject* i_game_object)
{
	ASSERT(i_game_object);
	SAFE_DELETE(game_object_);
	game_object_ = i_game_object;
}

inline MoveDirections PlayerController::GetMoveDirection() const
{
	return move_direction_;
}

inline void PlayerController::SetMoveDirection(MoveDirections i_move_direction)
{
	move_direction_ = i_move_direction;
}

} // namespace monsterchase