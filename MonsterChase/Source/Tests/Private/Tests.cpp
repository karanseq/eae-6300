/************************ MEMORY TESTS ************************/
#ifdef ENABLE_ALLOCATOR_TEST
#include "Tests\BlockAllocatorTest.h"
#include <stdio.h>
#include <conio.h>

// engine includes
#include "Memory\BlockAllocator.h"
#include "Memory\AllocatorUtil.h"
#include "Memory\AllocatorOverrides.h"
#include "Math\MathUtil.h"

bool HeapManager_UnitTest();

void TestFixedSizeAllocator();
#endif // ENABLE_ALLOCATOR_TEST

/************************ ENABLE OTHER TESTS ************************/

//#define ENABLE_VECTOR_CONST_TEST
//#define ENABLE_FLOAT_VALIDITY_TEST
//#define ENABLE_MOVE_SEMANTICS_TEST
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
void BitArray_UnitTest();
#endif // ENABLE_BIT_ARRAY_TEST


/************************ RUN TESTS ************************/

void RunTests()
{
#ifdef ENABLE_VECTOR_CONST_TEST
	TestVectorConstness();
#endif

#ifdef ENABLE_FLOAT_VALIDITY_TEST
	TestFloatValidity();
#endif

#ifdef ENABLE_MOVE_SEMANTICS_TEST
	TestMoveSemantics();
#endif

#ifdef ENABLE_BIT_ARRAY_TEST
	BitArray_UnitTest();
#endif

#ifdef ENABLE_ALLOCATOR_TEST
	TestFixedSizeAllocator();
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