#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

namespace engine {
namespace memory {

	void CreateAllocators()
	{
		// initialize the default allocator
		BlockAllocator* default_allocator = BlockAllocator::GetDefaultAllocator();

		// initialize the fixed size allocators
		// IMPORTANT: ensure that these are in ascending order of the size of the blocks they manage
		// the new & delete operators rely on this order to deallocate from the correct FSA
		FixedSizeAllocator* fsa = FixedSizeAllocator::Create(8, 32, default_allocator);
		FixedSizeAllocator::RegisterFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(16, 56, default_allocator);
		FixedSizeAllocator::RegisterFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(36, 24, default_allocator);
		FixedSizeAllocator::RegisterFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(76, 20, default_allocator);
		FixedSizeAllocator::RegisterFixedSizeAllocator(fsa);

		fsa = FixedSizeAllocator::Create(252, 20, default_allocator);
		FixedSizeAllocator::RegisterFixedSizeAllocator(fsa);
	}

	void DestroyAllocators()
	{
		// destroy the fixed size allocators
		FixedSizeAllocator** const registered_fsas = FixedSizeAllocator::GetRegisteredFixedSizeAllocators();

		for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
		{
			if (registered_fsas[i])
			{
				FixedSizeAllocator::Destroy(registered_fsas[i]);
				registered_fsas[i] = nullptr;
			}
		}

		// destroy the default allocator
		BlockAllocator::DestroyDefaultAllocator();
	}

} // namespace memory
} // namespace engine