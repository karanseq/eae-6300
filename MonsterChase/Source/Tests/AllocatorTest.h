#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

#include "Memory/BlockAllocator.h"

class AllocatorTest
{
private:
	AllocatorTest() {};
	~AllocatorTest() {};
	AllocatorTest(const AllocatorTest& i_copy);
	AllocatorTest& operator=(const AllocatorTest& i_at);

	static char* DoAlloc(const size_t i_size);
	static void DoFree(char* i_pointer, const size_t i_size);

public:
	static void Init(const size_t i_total_memory);
	static void Reset();

	static void RunTest00();
	static void RunTest01();
	static void RunTest02();
	static void RunTest03();

private:
	static uint8_t*									memory_;
	static engine::memory::BlockAllocator*			block_allocator_;
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
