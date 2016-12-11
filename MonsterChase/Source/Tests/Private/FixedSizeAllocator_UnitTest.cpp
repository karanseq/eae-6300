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

	// run first pass
	// make <num_blocks> allocs
	const size_t num_blocks = fsa->GetNumBlocks();
	for (size_t i = 0; i < num_blocks; ++i)
	{
		void* pointer = fsa->Alloc();
		ASSERT(pointer);
		allocations.push_back(pointer);
		LOG("Alloc-%zu num available blocks:%zu", i, fsa->GetNumAvailableBlocks());
	}

	ASSERT(!fsa->Alloc());
	ASSERT(fsa->GetNumOustandingBlocks() == num_blocks);
	ASSERT(fsa->GetNumAvailableBlocks() == 0);

	// free <num_blocks> in random order
	std::random_shuffle(allocations.begin(), allocations.end());
	for (size_t i = 0; i < num_blocks; ++i)
	{
		void* pointer = allocations.back();
		allocations.pop_back();

		bool success = fsa->Contains(pointer);
		assert(success);

		//success = IsAllocated( pHeapManager, pPtr );
		success = fsa->IsAllocated(pointer);
		assert(success);

		success = fsa->Free(pointer);
		assert(success);
		LOG("Free-%zu num outstanding blocks:%zu", i, fsa->GetNumOustandingBlocks());
	}

	ASSERT(fsa->GetNumAvailableBlocks() == num_blocks);
	ASSERT(fsa->GetNumOustandingBlocks() == 0);

	// run second pass
	size_t num_allocs = 0, num_frees = 0;
	do
	{
		const uint8_t free_every = 10;

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
			LOG("Free-%zu num outstanding blocks:%zu", num_frees, fsa->GetNumOustandingBlocks());
		}
		else
		{
			allocations.push_back(pointer);
		}

		LOG("Alloc-%zu num available blocks:%zu", num_allocs, fsa->GetNumAvailableBlocks());

	} while (true);

	ASSERT(fsa->GetNumOustandingBlocks() == num_blocks);
	ASSERT(fsa->GetNumAvailableBlocks() == 0);

	size_t unfreed_allocations = allocations.size();
	while (unfreed_allocations-- > 0)
	{
		void* pointer = allocations.back();
		allocations.pop_back();

		bool success = fsa->Contains(pointer);
		assert(success);

		//success = IsAllocated( pHeapManager, pPtr );
		success = fsa->IsAllocated(pointer);
		assert(success);

		success = fsa->Free(pointer);
		assert(success);
	}

	ASSERT(fsa->GetNumAvailableBlocks() == num_blocks);
	ASSERT(fsa->GetNumOustandingBlocks() == 0);

	LOG("-------------------- Finished exhausting FixedSizeAllocator block_size:%zu num_blocks:%zu --------------------", fsa->GetBlockSize(), fsa->GetNumBlocks());
}

void TestFixedSizeAllocator()
{
	engine::memory::BlockAllocator*				default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();

	engine::memory::FixedSizeAllocator*			fsa_48 = engine::memory::FixedSizeAllocator::Create(48, 1024, default_allocator);
	ExhaustAllocator(fsa_48);
	engine::memory::FixedSizeAllocator::Destroy(fsa_48);
}
