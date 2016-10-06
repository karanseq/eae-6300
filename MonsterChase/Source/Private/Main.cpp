// include gameplay elements
#include "MonsterChase.h"

#ifdef ENABLE_ALLOCATOR_TEST
#include "AllocatorTest.h"
#include <stdio.h>
#include <conio.h>
#endif

int main(int* argv, char** argc)
{
#ifdef ENABLE_ALLOCATOR_TEST
	printf("Beginning allocator test...\nCheck the Output window for details.\n");
	AllocatorTest::Init(1024 * 100, 128);
	AllocatorTest::RunTest01();
	AllocatorTest::RunTest02();
	AllocatorTest::RunTest03();
	AllocatorTest::Reset();
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

