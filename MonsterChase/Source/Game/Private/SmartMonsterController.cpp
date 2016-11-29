#include "Game\SmartMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\MonsterChase.h"

SmartMonsterController::SmartMonsterController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject()),
	target_(nullptr)
{}

SmartMonsterController::SmartMonsterController(engine::gameobject::GameObject* game_object) : game_object_(game_object),
	target_(nullptr)
{}

SmartMonsterController::~SmartMonsterController()
{
	SAFE_DELETE(game_object_);
}

SmartMonsterController::SmartMonsterController(const SmartMonsterController& copy) : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(copy.game_object_->GetTransform())),
	target_(copy.target_)
{}

SmartMonsterController::SmartMonsterController(SmartMonsterController&& copy) : game_object_(copy.game_object_),
	target_(copy.target_)
{
	copy.game_object_ = nullptr;
}

SmartMonsterController* SmartMonsterController::Clone() const
{
	// first clone the game object this controller maintains
	engine::gameobject::GameObject* game_object_clone = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(game_object_->GetTransform());
	// now create a new controller with the cloned game object
	SmartMonsterController* controller = new (MonsterChase::GetAllocator()) SmartMonsterController(game_object_clone);
	controller->target_ = target_;
	return controller;
}

void SmartMonsterController::UpdateGameObject()
{
	ASSERT(target_);

	const engine::math::Vec3D& my_position = game_object_->GetPosition();
	const engine::math::Vec3D& target_position = target_->GetPosition();

	if (my_position.y() < target_position.y())
	{
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_Y);
	}
	else if (my_position.y() > target_position.y())
	{
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_Y);
	}
	else if (my_position.x() < target_position.x())
	{
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_X);
	}
	else if (my_position.x() > target_position.x())
	{
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_X);
	}
}