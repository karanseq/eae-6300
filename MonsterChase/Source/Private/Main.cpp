// include gameplay elements
#include "MonsterChase.h"

#ifdef ENABLE_ALLOCATOR_TEST
#include "AllocatorTest.h"
#include <stdio.h>
#include <conio.h>

bool HeapManager_UnitTest();
#endif

#define ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_VECTOR_CONST_TEST
void TestVectorConstness();
#endif

int main(int* argv, char** argc)
{
#ifdef ENABLE_VECTOR_CONST_TEST
	TestVectorConstness();
#endif

#ifdef ENABLE_ALLOCATOR_TEST
	printf("Beginning allocator test...\nCheck the Output window for details.\n");
#ifdef BUILD_DEBUG
	HeapManager_UnitTest();
#else
	AllocatorTest::Init(1024 * 1024);
	AllocatorTest::RunTest00();
	AllocatorTest::RunTest01();
	AllocatorTest::RunTest02();
	AllocatorTest::RunTest03();
	AllocatorTest::Reset();
#endif
	printf("Finished test.\n");
	_getch();
	return 0;
#endif

	// initialize game
	MonsterChase monster_chase;

	while (monster_chase.GetState() != GameStateQuit)
	{
		monster_chase.Update();
	}

	return 0;
}

