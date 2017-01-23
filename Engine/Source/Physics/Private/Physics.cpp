#include "Physics\Physics.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"

namespace engine {
namespace physics {

// static member initialization
Physics* Physics::instance_ = nullptr;

Physics::Physics()
{}

Physics::~Physics()
{}

Physics* Physics::Create()
{
	if (!Physics::instance_)
	{
		Physics::instance_ = new Physics();
	}
	return Physics::instance_;
}

void Physics::Destroy()
{
	SAFE_DELETE(Physics::instance_);
}

void Physics::Run(float dt)
{
	VERBOSE("%s DT:%f", __FUNCTION__, dt);
}

} // namespace physics
} // namespace engine
