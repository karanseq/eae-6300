// library includes
#include <vector>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"

void ExhaustAllocator(engine::memory::FixedSizeAllocator* fsa)
{
	ASSERT(fsa);
	LOG("-------------------- Exhausting FixedSizeAllocator block_size:%zu num_blocks:%zu --------------------", fsa->GetBlockSize(), fsa->GetNumBlocks());
	std::vector<void*> allocations;

	size_t num_allocs = 0, num_frees = 0;

	do
	{
		const uint8_t free_every = 5;

		void* pointer = fsa->Alloc();
		if (!pointer)
		{
			break;
		}

		++num_allocs;
		memset(pointer, 65, fsa->GetBlockSize());

		if ((num_allocs % free_every) == 0)
		{
			++num_frees;
			fsa->Free(pointer);
		}
		else
		{
			allocations.push_back(pointer);
		}

	} while (true);

	size_t unfreed_allocations = allocations.size();
	while (unfreed_allocations-- > 0)
	{
		void* pointer = allocations.back();
		fsa->Free(pointer);
		allocations.pop_back();
	}

	LOG("-------------------- Finished exhausting FixedSizeAllocator block_size:%zu num_blocks:%zu num_allocs:%zu num_frees:%zu --------------------", fsa->GetBlockSize(), fsa->GetNumBlocks(), num_allocs, num_frees);
}

void TestFixedSizeAllocator()
{
	engine::memory::BlockAllocator*				default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();

	engine::memory::FixedSizeAllocator*			fsa_8 = engine::memory::FixedSizeAllocator::Create(8, 128, default_allocator);
	ExhaustAllocator(fsa_8);
	engine::memory::FixedSizeAllocator::Destroy(fsa_8);

	engine::memory::FixedSizeAllocator*			fsa_24 = engine::memory::FixedSizeAllocator::Create(24, 512, default_allocator);
	ExhaustAllocator(fsa_24);
	engine::memory::FixedSizeAllocator::Destroy(fsa_24);

	engine::memory::FixedSizeAllocator*			fsa_48 = engine::memory::FixedSizeAllocator::Create(48, 1024, default_allocator);
	ExhaustAllocator(fsa_48);
	engine::memory::FixedSizeAllocator::Destroy(fsa_48);

	engine::memory::FixedSizeAllocator*			fsa_128 = engine::memory::FixedSizeAllocator::Create(128, 256, default_allocator);
	ExhaustAllocator(fsa_128);
	engine::memory::FixedSizeAllocator::Destroy(fsa_128);

	engine::memory::FixedSizeAllocator*			fsa_256 = engine::memory::FixedSizeAllocator::Create(256, 128, default_allocator);
	ExhaustAllocator(fsa_256);
	engine::memory::FixedSizeAllocator::Destroy(fsa_256);
}
