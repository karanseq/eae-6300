#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

#include "Allocator/BlockAllocator.h"

class AllocatorTest
{
private:
	AllocatorTest() {};
	~AllocatorTest() {};

public:
	static void Init(const size_t total_memory = 1024 * 1024, const unsigned int num_bds = 32);
	static void Reset();

	static void RunTest01();
	static void RunTest02();

private:
	static engine::BlockAllocator* block_allocator_;
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
