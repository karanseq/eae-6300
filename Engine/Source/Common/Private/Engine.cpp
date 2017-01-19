#include "Common\Engine.h"

// library includes
#include <stdlib.h>
#include <time.h>

// engine includes
#include "Memory\AllocatorUtil.h"

namespace engine {

bool StartUp()
{
	// create allocators
	engine::memory::CreateAllocators();

	// give rand a new seed
	srand(static_cast<unsigned int>(time(0)));

	return true;
}

void Shutdown()
{
	// delete allocators
	engine::memory::DestroyAllocators();
}

} // namespace engine