#include "Game\SillyMonsterController.h"

// engine includes
#include "Memory\AllocatorOverrides.h"

// game includes
#include "Game\GameTypes.h"
#include "Game\GameUtils.h"
#include "Game\MonsterChase.h"

SillyMonsterController::SillyMonsterController() : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject())
{}

SillyMonsterController::SillyMonsterController(engine::gameobject::GameObject* i_game_object) : game_object_(i_game_object)
{}

SillyMonsterController::~SillyMonsterController()
{
	SAFE_DELETE(game_object_);
}

SillyMonsterController::SillyMonsterController(const SillyMonsterController& i_copy) : game_object_(new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(i_copy.game_object_->GetTransform()))
{}

SillyMonsterController::SillyMonsterController(SillyMonsterController&& i_copy) : game_object_(i_copy.game_object_)
{
	i_copy.game_object_ = nullptr;
}

SillyMonsterController* SillyMonsterController::Clone() const
{
	// first clone the game object this controller maintains
	engine::gameobject::GameObject* game_object_clone = new (MonsterChase::GetAllocator()) engine::gameobject::GameObject(game_object_->GetTransform());
	// now create a new controller with the cloned game object
	SillyMonsterController* controller = new (MonsterChase::GetAllocator()) SillyMonsterController(game_object_clone);
	return controller;
}

void SillyMonsterController::UpdateGameObject()
{
	MoveDirections direction = GameUtils::GetRandomDirection();
	switch (direction)
	{
	case MoveDirections::kMoveDirectionLeft:
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionRight:
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_X);
		break;

	case MoveDirections::kMoveDirectionUp:
		game_object_->SetPosition(game_object_->GetPosition() + engine::math::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionDown:
		game_object_->SetPosition(game_object_->GetPosition() - engine::math::Vec3D::UNIT_Y);
		break;

	case MoveDirections::kMoveDirectionNone:
	default:
		break;
	}
}
