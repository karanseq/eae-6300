#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

#include "Allocator/BlockAllocator.h"

class AllocatorTest
{
private:
	AllocatorTest() {};
	~AllocatorTest() {};

	static char* DoAlloc(const size_t size);
	static void DoFree(char* pointer, const size_t size);

public:
	static void Init(const size_t total_memory = 1024 * 1024, const unsigned int num_bds = 1024);
	static void Reset();

	static void RunTest01();
	static void RunTest02();
	static void RunTest03();

private:
	static engine::BlockAllocator* block_allocator_;
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
