#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

// game includes
#include "Game\GameTypes.h"

// engine includes
#include "GameObject\InterfaceGameObjectController.h"
#include "GameObject\GameObject.h"
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

class PlayerController : public engine::gameobject::InterfaceGameObjectController
{
public:
	PlayerController();
	virtual ~PlayerController();

	/* Implement InterfaceGameObjectController */
	inline engine::gameobject::GameObject* GetGameObject() override								{ return game_object_; }
	inline void SetGameObject(engine::gameobject::GameObject* game_object) override				{ ASSERT(game_object); SAFE_DELETE(game_object_); game_object_ = game_object; }

	void UpdateGameObject() override;

	inline MoveDirections GetMoveDirection() const												{ return move_direction_; }
	inline void SetMoveDirection(MoveDirections move_direction)									{ move_direction_ = move_direction; }

private:
	// disable default copy constructor
	PlayerController(const PlayerController& copy);
	// disable default assignment operator
	PlayerController& operator=(const PlayerController& monster);

private:
	engine::gameobject::GameObject*					game_object_;
	MoveDirections									move_direction_;

}; // class PlayerController

#endif // PLAYER_CONTROLLER_H_
