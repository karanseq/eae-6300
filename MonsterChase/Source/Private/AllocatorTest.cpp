#ifdef ENABLE_ALLOCATOR_TEST

#include "AllocatorTest.h"
#include "Allocator/BlockAllocator.h"
#include "Logger/Logger.h"

#include <stdlib.h>

void AllocatorTest::RunTest(size_t total_memory, unsigned int num_bds)
{
	LOG_DEBUG("Testing BlockAllocator TOTAL_MEM:%zu NUM_BDS:%d", total_memory, num_bds);

	engine::BlockAllocator* block_allocator = engine::BlockAllocator::Create(total_memory, num_bds);
#ifdef BUILD_DEBUG
	block_allocator->PrintAllDescriptors();
#endif

	const size_t num_pointers = 25;
	char* pointers[num_pointers] = { 0 };

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		size_t rand_size = 1 + rand() % 8;
		pointers[i] = (char*)block_allocator->Alloc(rand_size);

		for (int j = 0; j < rand_size; ++j)
		{
			pointers[i][j] = 65 + i;
		}
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
