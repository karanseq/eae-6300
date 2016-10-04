#ifdef ENABLE_ALLOCATOR_TEST

#ifndef ALLOCATOR_TEST_H_
#define ALLOCATOR_TEST_H_

class AllocatorTest
{
private:
	AllocatorTest() {};
	~AllocatorTest() {};

public:
	static void RunTest(const size_t total_memory = 1024 * 1024, const unsigned int num_bds = 32);
};

#endif // ALLOCATOR_TEST_H_

#endif // ENABLE_ALLOCATOR_TEST
