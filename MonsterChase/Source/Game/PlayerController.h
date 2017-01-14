#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

// engine includes
#include "Assert\Assert.h"
#include "GameObject\GameObject.h"
#include "GameObject\InterfaceGameObjectController.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\GameTypes.h"

namespace monsterchase {

class PlayerController : public engine::gameobject::InterfaceGameObjectController
{
public:
	PlayerController();
	PlayerController(engine::gameobject::GameObject* i_game_object);
	virtual ~PlayerController();

	// copy constructor
	PlayerController(const PlayerController& i_copy);
	// move constructor
	PlayerController(PlayerController&& i_copy);

	// copy assignment operator
	inline PlayerController& operator=(const PlayerController& i_controller);
	// move assignment operator
	inline PlayerController& operator=(PlayerController&& i_controller);

	/* Implement InterfaceGameObjectController */
	PlayerController* Clone() const override;
	inline engine::gameobject::GameObject* GetGameObject();
	inline void SetGameObject(engine::gameobject::GameObject* i_game_object);

	void UpdateGameObject() override;

	inline MoveDirections GetMoveDirection() const;
	inline void SetMoveDirection(MoveDirections i_move_direction);

private:
	engine::gameobject::GameObject*					game_object_;
	MoveDirections									move_direction_;

}; // class PlayerController

} // namespace monsterchase

#include "PlayerController-inl.h"

#endif // PLAYER_CONTROLLER_H_
