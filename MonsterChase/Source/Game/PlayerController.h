#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\GameTypes.h"

class PlayerController : public engine::gameobject::InterfaceGameObjectController
{
public:
	PlayerController();
	virtual ~PlayerController();

	/* Implement InterfaceGameObjectController */
	inline engine::gameobject::GameObject* GetGameObject() override									{ return game_object_; }
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object) override				{ ASSERT(i_game_object); SAFE_DELETE(game_object_); game_object_ = i_game_object; }

	void UpdateGameObject() override;

	inline MoveDirections GetMoveDirection() const													{ return move_direction_; }
	inline void SetMoveDirection(MoveDirections i_move_direction)									{ move_direction_ = i_move_direction; }

private:
	// disable default copy constructor
	PlayerController(const PlayerController& i_copy);
	// disable default assignment operator
	PlayerController& operator=(const PlayerController& i_controller);

private:
	engine::gameobject::GameObject*					game_object_;
	MoveDirections									move_direction_;

}; // class PlayerController

#endif // PLAYER_CONTROLLER_H_
