#ifndef MONSTER_CONTROLLER_H_
#define MONSTER_CONTROLLER_H_

#include "GameObject\GameObjectController.h"
#include "GameObject\MovementController.h"
#include "GameObject\NameController.h"
#include "GameObject\DebugPrintController.h"
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"

#include <stdio.h>

class MonsterController : public engine::GameObjectController,
	public engine::MovementController,
	public engine::NameController,
	public engine::DebugPrintController
{
public:
	MonsterController() : monster_(nullptr),
		target_(nullptr),
		name_(nullptr),
		time_to_live_(0)
	{}
	virtual ~MonsterController();

	/* Implement the GameObjectController */
	inline void SetGameObject(engine::GameObject* game_object) override			{ monster_ = game_object; }
	inline engine::GameObject* GetGameObject() override							{ return monster_; }

	void UpdateGameObject() override;

	/* Implement the MovementController */
	void Move(engine::MovementController::MoveDirection2D direction) override;

	/* Implement the NameController */
	void SetName(const char* name) override;
	inline const char* GetName() const override					{ return name_; }

	/* Implement the DebugPrintController */
	inline void DebugPrint() const override						{ ASSERT(monster_ != nullptr); printf("Monster %s %s [%f, %f]\n", name_, (time_to_live_ <= 1 ? "is about to die at" : "is at"), monster_->GetPosition().x(), monster_->GetPosition().y()); }

	inline void SetTarget(engine::GameObject* target)			{ target_ = target; }
	inline engine::GameObject* GetTarget()						{ return target_; }

	inline void SetPosition(const engine::Vec2D& position)		{ ASSERT(monster_ != nullptr); monster_->SetPosition(position); }
	inline const engine::Vec2D& GetPosition() const				{ ASSERT(monster_ != nullptr); return monster_->GetPosition(); }

	inline int GetTimeToLive() const							{ return time_to_live_; }
	inline void SetTimeToLive(int time_to_live)					{ time_to_live_ = time_to_live; }

private:
	engine::GameObject* monster_;
	engine::GameObject* target_;
	const char* name_;

	int time_to_live_;
};

#endif // MONSTER_CONTROLLER_H_
