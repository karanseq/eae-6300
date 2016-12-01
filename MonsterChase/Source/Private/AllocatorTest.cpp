#ifdef ENABLE_ALLOCATOR_TEST

#include "AllocatorTest.h"
#include "Logger/Logger.h"

#include <stdlib.h>
#include <vector>		// required ONLY for test03

//#define SIMULATE_MEMORY_OVERWRITE

engine::BlockAllocator* AllocatorTest::block_allocator_ = NULL;

void AllocatorTest::Init(size_t total_memory, unsigned int num_bds)
{
	LOG("Testing BlockAllocator TOTAL_MEM:%zu NUM_BDS:%d", total_memory, num_bds);

	block_allocator_ = engine::BlockAllocator::Create(total_memory, num_bds);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
}

void AllocatorTest::Reset()
{
	LOG("Destroying BlockAllocator...");
	block_allocator_->Destroy();
}

uint8_t* AllocatorTest::DoAlloc(const size_t size)
{
	uint8_t* ret = static_cast<uint8_t*>(block_allocator_->Alloc(size));
	LOG("Alloc-%zu", size);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
	return ret;
}

void AllocatorTest::DoFree(uint8_t* pointer, const size_t size)
{
	block_allocator_->Free(pointer);
	LOG("Free-%zu", size);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
}

void AllocatorTest::RunTest01()
{
	LOG("-------------------- Running Test 01 --------------------");

	const size_t num_pointers = 25;
	char* pointers[num_pointers] = { 0 };

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		size_t rand_size = 1 + rand() % 512;
		pointers[i] = static_cast<char*>(block_allocator_->Alloc(rand_size));

		for (uint16_t j = 0; j < rand_size; ++j)
		{
			pointers[i][j] = 65 + i;
		}

#ifdef SIMULATE_MEMORY_OVERWRITE
		// write beyond what we requested
		if (i % 3)
		{
			pointers[i][rand_size] = 65 + i;
			pointers[i][rand_size + 1] = 65 + i;
		}
#endif
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		if (pointers[i] != NULL)
		{
			LOG("pointer[%d] = %p : %s", i, pointers[i], pointers[i]);
		}
		else
		{
			LOG("pointer[%d] = NULL", i);
		}
	}

	for (uint8_t i = 0; i < num_pointers; ++i)
	{
		block_allocator_->Free(pointers[i]);
		pointers[i] = NULL;
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 01 --------------------");
}

void AllocatorTest::RunTest02()
{
	LOG("-------------------- Running Test 02 --------------------");

	uint8_t* buf1 = DoAlloc(9);
	uint8_t* buf2 = DoAlloc(23);
	
	DoFree(buf1, 9);
	DoFree(buf2, 23);

	buf2 = DoAlloc(10);
	buf1 = DoAlloc(22);

	DoFree(buf1, 10);
	DoFree(buf2, 22);

	block_allocator_->Defragment();
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 02 --------------------");
}

void AllocatorTest::RunTest03()
{
	LOG("-------------------- Running Test 03 --------------------");
	const uint16_t iterations = 512;
	const uint16_t max_size = 1024 * 5;
	bool successful = false;

	std::vector<uint8_t*> unfreed_pointers;

	for (uint16_t i = 0; i < iterations; ++i)
	{
		const size_t rand_size = static_cast<size_t>(rand() % max_size);
		LOG("Request-%d Alloc size:%zu", i, rand_size);
		uint8_t* buf = static_cast<uint8_t*>(block_allocator_->Alloc(rand_size));
		successful = (buf != NULL);

		if (successful && (rand() % 10) > 3)
		{
			LOG("Request-%d Free size:%zu", i, rand_size);
			block_allocator_->Free(buf);
		}
		else if (successful)
		{
			unfreed_pointers.push_back(buf);
		}

		if (successful)
		{
#ifdef BUILD_DEBUG
			block_allocator_->PrintAllDescriptors();
#endif
		}
	}

	const size_t num_unfreed_pointers = unfreed_pointers.size();
	LOG("Freeing %zu user allocations...", num_unfreed_pointers);
	for (uint16_t i = 0; i < num_unfreed_pointers; ++i)
	{
		block_allocator_->Free(unfreed_pointers[i]);
	}
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	block_allocator_->Defragment();
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG("-------------------- Finished Test 03 --------------------");
}

#endif // ENABLE_ALLOCATOR_TEST
