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
	PlayerController(engine::gameobject::GameObject* game_object);
	virtual ~PlayerController();

	// copy constructor
	PlayerController(const PlayerController& copy);
	// move constructor
	PlayerController(PlayerController&& copy);

	// copy assignment operator
	inline PlayerController& operator=(const PlayerController& controller);
	// move assignment operator
	inline PlayerController& operator=(PlayerController&& controller);

	/* Implement InterfaceGameObjectController */
	PlayerController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject();
	inline void SetGameObject(engine::gameobject::GameObject* game_object);

	void UpdateGameObject() override;

	inline MoveDirections GetMoveDirection() const;
	inline void SetMoveDirection(MoveDirections move_direction);

private:
	engine::gameobject::GameObject*					game_object_;
	MoveDirections									move_direction_;

}; // class PlayerController

#include "PlayerController-inl.h"

#endif // PLAYER_CONTROLLER_H_
