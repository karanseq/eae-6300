#include "Game\Player.h"

// engine includes
#include "GameObject\Actor.h"
#include "GameObject\GameObject.h"
#include "Physics\Physics.h"
#include "Renderer\Renderer.h"

// game includes
#include "Game\GameData.h"

namespace monsterchase
{

// static member initialization
const float Player::DEFAULT_MASS = 50.0f;

Player::Player() : actor_(nullptr)
{
	auto game_object = engine::gameobject::GameObject::Create();
	actor_ = new engine::gameobject::Actor(game_object, engine::physics::Physics::Get()->CreatePhysicsObject(game_object), engine::render::Renderer::Get()->CreateRenderableObject(GameData::PLAYER_TEXTURE_NAME, game_object));
}

Player::~Player()
{}

} // namespace monsterchase
