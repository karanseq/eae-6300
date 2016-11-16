#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

// game includes
#include "Game\GameTypes.h"

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class PlayerController : public engine::InterfaceGameObjectController
{
public:
	PlayerController() : game_object_(new engine::GameObject()),
		move_direction_(MoveDirections::kMoveDirectionNone)
	{}

	virtual ~PlayerController()
	{
		SAFE_DELETE(game_object_);
	}

	/* Implement InterfaceGameObjectController */
	inline engine::GameObject* GetGameObject() override								{ return game_object_; }
	inline void SetGameObject(engine::GameObject* game_object) override				{ ASSERT(game_object); SAFE_DELETE(game_object_); game_object_ = game_object; }

	void UpdateGameObject() override;

	inline MoveDirections GetMoveDirection() const									{ return move_direction_; }
	inline void SetMoveDirection(MoveDirections move_direction)						{ move_direction_ = move_direction; }

private:
	engine::GameObject* game_object_;
	MoveDirections move_direction_;

}; // class PlayerController

#endif // PLAYER_CONTROLLER_H_
