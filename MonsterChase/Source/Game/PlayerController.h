#ifndef PLAYER_CONTROLLER_H_
#define PLAYER_CONTROLLER_H_

#include "GameObject\GameObjectController.h"
#include "GameObject\MovementController.h"
#include "GameObject\NameController.h"
#include "GameObject\DebugPrintController.h"
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"

#include <stdio.h>

class PlayerController : public engine::GameObjectController, 
	public engine::MovementController, 
	public engine::NameController,
	public engine::DebugPrintController
{
public:
	PlayerController() : player_(nullptr),
		name_(nullptr)
	{}
	virtual ~PlayerController();

	/* Implement the GameObjectController */
	inline void SetGameObject(engine::GameObject* game_object) override			{ player_ = game_object; }
	inline engine::GameObject* GetGameObject() override							{ return player_; }

	void UpdateGameObject() override;

	/* Implement the MovementController */
	void Move(engine::MovementController::MoveDirection2D direction) override;

	/* Implement the NameController */
	void SetName(const char* name) override;
	inline const char* GetName() const override					{ return name_; }

	/* Implement the DebugPrintController */
	inline void DebugPrint() const override						{ ASSERT(player_ != nullptr); printf("Player %s is at [%f, %f]\n", name_, player_->GetPosition().x(), player_->GetPosition().y()); }

	inline void SetPosition(const engine::Vec2D& position)		{ ASSERT(player_ != nullptr); player_->SetPosition(position); }
	inline const engine::Vec2D& GetPosition() const				{ ASSERT(player_ != nullptr); return player_->GetPosition(); }

private:
	engine::GameObject* player_;
	const char* name_;
};

#endif // PLAYER_CONTROLLER_H_
