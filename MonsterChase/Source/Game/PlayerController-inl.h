#include "PlayerController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "MonsterChase.h"

inline PlayerController& PlayerController::operator=(const PlayerController& controller)
{
	if (this != &controller)
	{
		SAFE_DELETE(game_object_);
		game_object_ = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(controller.game_object_->GetTransform());
		move_direction_ = controller.move_direction_;
	}
	return *this;
}

inline PlayerController& PlayerController::operator=(PlayerController&& controller)
{
	if (this != &controller)
	{
		std::swap(game_object_, controller.game_object_);
		move_direction_ = controller.move_direction_;
	}
	return *this;
}

inline engine::gameobject::GameObject* PlayerController::GetGameObject()
{
	return game_object_;
}

inline void PlayerController::SetGameObject(engine::gameobject::GameObject* game_object)
{
	ASSERT(game_object);
	SAFE_DELETE(game_object_);
	game_object_ = game_object;
}

inline MoveDirections PlayerController::GetMoveDirection() const
{
	return move_direction_;
}

inline void PlayerController::SetMoveDirection(MoveDirections move_direction)
{
	move_direction_ = move_direction;
}