/************************ MEMORY TESTS ************************/
#ifdef ENABLE_ALLOCATOR_TEST
#include "Tests\BlockAllocatorTest.h"

// engine includes
#include "Logger\Logger.h"

bool HeapManager_UnitTest();

void TestFixedSizeAllocator();
#endif // ENABLE_ALLOCATOR_TEST

/************************ ENABLE OTHER TESTS ************************/

#define ENABLE_VECTOR_CONST_TEST
#define ENABLE_FLOAT_VALIDITY_TEST
#define ENABLE_MOVE_SEMANTICS_TEST
#define ENABLE_BIT_ARRAY_TEST

#ifdef ENABLE_VECTOR_CONST_TEST
void TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_FLOAT_VALIDITY_TEST
void TestFloatValidity();
#endif // ENABLE_FLOAT_VALIDITY_TEST

#ifdef ENABLE_MOVE_SEMANTICS_TEST
void TestMoveSemantics();
#endif // ENABLE_MOVE_SEMANTICS_TEST

#ifdef ENABLE_BIT_ARRAY_TEST
void RunBitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST


/************************ RUN TESTS ************************/

void RunTests()
{
	LOG("\n");
#ifdef ENABLE_VECTOR_CONST_TEST
	TestVectorConstness();
#endif

	LOG("\n");
#ifdef ENABLE_FLOAT_VALIDITY_TEST
	TestFloatValidity();
#endif

	LOG("\n");
#ifdef ENABLE_MOVE_SEMANTICS_TEST
	TestMoveSemantics();
#endif

	LOG("\n");
#ifdef ENABLE_BIT_ARRAY_TEST
	RunBitArray_UnitTest();
#endif

	LOG("\n");
#ifdef ENABLE_ALLOCATOR_TEST
	TestFixedSizeAllocator();
	LOG("\n");
#ifdef BUILD_DEBUG
	HeapManager_UnitTest();
#else
	BlockAllocatorTest::Init();
	BlockAllocatorTest::RunTest00();
	BlockAllocatorTest::RunTest01();
	BlockAllocatorTest::RunTest02();
	BlockAllocatorTest::RunTest03();
	BlockAllocatorTest::Reset();
#endif
#endif
}