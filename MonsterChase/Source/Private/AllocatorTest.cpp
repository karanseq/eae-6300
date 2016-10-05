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

	char* buf1 = (char*)block_allocator_->Alloc(32);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Alloc-32");
	block_allocator_->PrintAllDescriptors();
	#endif
	
	char* buf2 = (char*)block_allocator_->Alloc(96);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Alloc-96");
	block_allocator_->PrintAllDescriptors();
	#endif
	
	block_allocator_->Free(buf1);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Free-32");
	block_allocator_->PrintAllDescriptors();
	#endif

	block_allocator_->Free(buf2);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Free-96");
	block_allocator_->PrintAllDescriptors();
	#endif

	buf2 = (char*)block_allocator_->Alloc(48);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Alloc-48");
	block_allocator_->PrintAllDescriptors();
	#endif

	buf1 = (char*)block_allocator_->Alloc(48);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Alloc-48");
	block_allocator_->PrintAllDescriptors();
	#endif

	block_allocator_->Free(buf1);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Free-48");
	block_allocator_->PrintAllDescriptors();
	#endif

	block_allocator_->Free(buf2);
	#ifdef BUILD_DEBUG
	LOG_DEBUG("Free-48");
	block_allocator_->PrintAllDescriptors();
	#endif

	LOG_DEBUG("-------------------- Finished Test 02 --------------------");
}

#endif // ENABLE_ALLOCATOR_TEST
