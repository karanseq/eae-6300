#ifdef ENABLE_ALLOCATOR_TEST

#include "AllocatorTest.h"
#include "Allocator/BlockAllocator.h"
#include "Logger/Logger.h"

#include <math.h>		// for pow

void AllocatorTest::RunTest(size_t total_memory, unsigned int num_bds)
{
	LOG_DEBUG("Testing BlockAllocator TOTAL_MEM:%zu NUM_BDS:%d", total_memory, num_bds);

	engine::BlockAllocator* block_allocator = engine::BlockAllocator::Create(total_memory, num_bds);
#ifdef BUILD_DEBUG
	block_allocator->PrintAllDescriptors();
#endif

	const size_t num_pointers = 20;
	char* pointers[num_pointers] = { 0 };

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		pointers[i] = (char*)block_allocator->Alloc((size_t)pow(2, i + 1));
	}

#ifdef BUILD_DEBUG
	block_allocator->PrintAllDescriptors();
#endif

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		if (pointers[i] != NULL)
		{
			LOG_DEBUG("pointer[%d] = %p", i, pointers[i]);
		}
		else
		{
			LOG_DEBUG("pointer[%d] = NULL", i);
		}
	}

	LOG_DEBUG("Largest block size:%zu", block_allocator->GetLargestFreeBlockSize());
	LOG_DEBUG("Total free memory:%zu", block_allocator->GetTotalFreeMemorySize());

	block_allocator->Destroy();
}

#endif // ENABLE_ALLOCATOR_TEST
