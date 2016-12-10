#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

#include "Memory/BlockAllocator.h"

class BlockAllocatorTest
{
private:
	BlockAllocatorTest() {};
	~BlockAllocatorTest() {};
	BlockAllocatorTest(const BlockAllocatorTest& copy);
	BlockAllocatorTest& operator=(const BlockAllocatorTest& at);

	static char* DoAlloc(const size_t size);
	static void DoFree(char* pointer, const size_t size);

public:
	static void Init(const size_t total_memory = 1024 * 1024);
	static void Reset();

	static void RunTest00();
	static void RunTest01();
	static void RunTest02();
	static void RunTest03();

private:
	static void*									memory_;
	static engine::memory::BlockAllocator*			block_allocator_;
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
