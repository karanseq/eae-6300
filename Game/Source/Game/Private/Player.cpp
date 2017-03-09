#include "Game\Player.h"

// engine includes
#include "Assert\Assert.h"
#include "GameObject\ActorCreator.h"
#include "Input\KeyboardEventDispatcher.h"
#include "Logger\Logger.h"
#include "Physics\Physics.h"
#include "Renderer\Renderer.h"
#include "Time\Updater.h"

// game includes
#include "Game\Game.h"
#include "Game\GameData.h"

namespace game
{

// static member initialization
const float Player::DEFAULT_MASS = 50.0f;
const float Player::DEFAULT_FORCE = 2.5f;

Player::Player() : is_left_pressed_(false),
	is_right_pressed_(false),
	is_up_pressed_(false),
	is_down_pressed_(false),
	actor_(nullptr),
	keyboard_event_(engine::input::KeyboardEvent::Create())
{
	engine::gameobject::ActorCreator::CreateActorFromFile(GameData::PLAYER_LUA_FILE_NAME, actor_);

	// register for update events
	engine::time::Updater::Get()->AddTickable(this);

	// register for key events
	ASSERT(keyboard_event_);
	keyboard_event_->SetOnKeyPressed(std::bind(&Player::OnKeyPressed, this, std::placeholders::_1));
	keyboard_event_->SetOnKeyReleased(std::bind(&Player::OnKeyReleased, this, std::placeholders::_1));
	engine::input::KeyboardEventDispatcher::Get()->AddListener(keyboard_event_);
}

Player::~Player()
{
	engine::input::KeyboardEventDispatcher::Get()->RemoveListener(keyboard_event_);
	keyboard_event_ = nullptr;

	engine::time::Updater::Get()->RemoveTickable(this);
}

void Player::Update(float i_dt)
{
	// apply an appropriate force if any of the keys are pressed
	if (is_left_pressed_ || is_right_pressed_ || is_up_pressed_ || is_down_pressed_)
	{
		engine::math::Vec3D force(is_left_pressed_ ? -Player::DEFAULT_FORCE : (is_right_pressed_ ? Player::DEFAULT_FORCE : 0.0f),
			is_down_pressed_ ? -Player::DEFAULT_FORCE : (is_up_pressed_ ? Player::DEFAULT_FORCE : 0.0f));

		actor_->GetPhysicsObject().Lock()->ApplyImpulse(force);
	}

	// wrap around the screen
	auto position = actor_->GetGameObject()->GetPosition();
	position.x((position.x() < -Game::SCREEN_WIDTH / 2) ? Game::SCREEN_WIDTH / 2 : (position.x() > Game::SCREEN_WIDTH / 2 ? -Game::SCREEN_WIDTH / 2 : position.x()));
	position.y((position.y() < -Game::SCREEN_HEIGHT / 2) ? Game::SCREEN_HEIGHT / 2 : (position.y() > Game::SCREEN_HEIGHT / 2 ? -Game::SCREEN_HEIGHT / 2 : position.y()));
	actor_->GetGameObject()->SetPosition(position);
}

void Player::OnKeyPressed(unsigned int i_key_id)
{
	is_left_pressed_ = is_left_pressed_ || i_key_id == 'A';
	is_right_pressed_ = is_right_pressed_ || i_key_id == 'D';
	is_down_pressed_ = is_down_pressed_ || i_key_id == 'S';
	is_up_pressed_ = is_up_pressed_ || i_key_id == 'W';
}

void Player::OnKeyReleased(unsigned int i_key_id)
{
	is_left_pressed_ = is_left_pressed_ ? !(i_key_id == 'A') : is_left_pressed_;
	is_right_pressed_ = is_right_pressed_ ? !(i_key_id == 'D') : is_right_pressed_;
	is_down_pressed_ = is_down_pressed_ ? !(i_key_id == 'S') : is_down_pressed_;
	is_up_pressed_ = is_up_pressed_ ? !(i_key_id == 'W') : is_up_pressed_;
}

} // namespace monsterchase