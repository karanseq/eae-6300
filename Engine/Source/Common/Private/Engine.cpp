#include "Common\Engine.h"

// library includes
#include <stdlib.h>
#include <time.h>

// engine includes
#include "Memory\AllocatorUtil.h"
#include "Time\TimerUtil.h"
#include "Time\Updater.h"

namespace engine {

bool StartUp()
{
	// create allocators
	engine::memory::CreateAllocators();

	// create updater
	engine::time::Updater::Create();

	// give rand a new seed
	// TODO: Resolve conflict with namespace time
	//srand(static_cast<unsigned int>(time(0)));

	quit_requested_ = false;

	return true;
}

void Run()
{
	while (!quit_requested_)
	{
		// get delta
		float dt = engine::time::TimerUtil::CalculateLastFrameTime_ms();

		// update modules
		engine::time::Updater::Get()->Run(dt);
	}
}

void Shutdown()
{
	// delete updater
	engine::time::Updater::Destroy();

	// delete allocators
	engine::memory::DestroyAllocators();
}

void RequestQuit()
{
	quit_requested_ = true;
}

} // namespace engine