#ifdef ENABLE_ALLOCATOR_TEST

#include "AllocatorTest.h"
#include "Logger/Logger.h"

#include <stdlib.h>

engine::BlockAllocator* AllocatorTest::block_allocator_ = NULL;

void AllocatorTest::Init(size_t total_memory, unsigned int num_bds)
{
	LOG_DEBUG("Testing BlockAllocator TOTAL_MEM:%zu NUM_BDS:%d", total_memory, num_bds);

	block_allocator_ = engine::BlockAllocator::Create(total_memory, num_bds);
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
}

void AllocatorTest::Reset()
{
#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif
	LOG_DEBUG("Destroying BlockAllocator...");
	block_allocator_->Destroy();
}

char* AllocatorTest::DoAlloc(const size_t size)
{
	char* ret = (char*)block_allocator_->Alloc(size);
#ifdef BUILD_DEBUG
	LOG_DEBUG("Alloc-%zu", size);
	block_allocator_->PrintAllDescriptors();
#endif
	return ret;
}

void AllocatorTest::DoFree(char* pointer, const size_t size)
{
	block_allocator_->Free(pointer);
#ifdef BUILD_DEBUG
	LOG_DEBUG("Free-%zu", size);
	block_allocator_->PrintAllDescriptors();
#endif
}

void AllocatorTest::RunTest01()
{
	LOG_DEBUG("-------------------- Running Test 01 --------------------");

	const size_t num_pointers = 25;
	char* pointers[num_pointers] = { 0 };

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		size_t rand_size = 1 + rand() % 512;
		//size_t rand_size = (i + 1) * 16;
		pointers[i] = (char*)block_allocator_->Alloc(rand_size);

		for (unsigned int j = 0; j < rand_size; ++j)
		{
			pointers[i][j] = 65 + i;
		}

		// write beyond what we requested
		if (i % 2)
		{
			pointers[i][rand_size] = 65 + i;
			pointers[i][rand_size + 1] = 65 + i;
		}
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		if (pointers[i] != NULL)
		{
			LOG_DEBUG("pointer[%d] = %p : %s", i, pointers[i], pointers[i]);
		}
		else
		{
			LOG_DEBUG("pointer[%d] = NULL", i);
		}
	}

	for (unsigned int i = 0; i < num_pointers; ++i)
	{
		block_allocator_->Free(pointers[i]);
		pointers[i] = NULL;
	}

#ifdef BUILD_DEBUG
	block_allocator_->PrintAllDescriptors();
#endif

	LOG_DEBUG("-------------------- Finished Test 01 --------------------");
}

void AllocatorTest::RunTest02()
{
	LOG_DEBUG("-------------------- Running Test 02 --------------------");

	char* buf1 = DoAlloc(21);
	char* buf2 = DoAlloc(96);
	
	DoFree(buf1, 21);
	DoFree(buf2, 96);

	buf2 = DoAlloc(48);
	buf1 = DoAlloc(48);

	DoFree(buf1, 48);
	DoFree(buf2, 48);

	LOG_DEBUG("-------------------- Finished Test 02 --------------------");
}

void AllocatorTest::RunTest03()
{
	const unsigned int iterations = 100;
	const unsigned int max_size = 1024 * 5;
	bool successful = false;
	for (unsigned int i = 0; i < iterations; ++i)
	{
		const size_t rand_size = (size_t)rand() % max_size;
		LOG_DEBUG("Request-%d Alloc size:%zu", i, rand_size);
		char* buf = (char*)block_allocator_->Alloc(rand_size);
		successful = (buf != NULL);

		if (successful && (rand() % 10) > 6)
		{
			LOG_DEBUG("Request-%d Free size:%zu", i, rand_size);
			block_allocator_->Free(buf);
		}

		if (successful)
		{
#ifdef BUILD_DEBUG
		block_allocator_->PrintAllDescriptors();
#endif
		}
	}
}

#endif // ENABLE_ALLOCATOR_TEST
