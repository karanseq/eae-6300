// include gameplay elements
#include "MonsterChase.h"

#ifdef ENABLE_ALLOCATOR_TEST
#include "AllocatorTest.h"
#endif

int main(int* argv, char** argc)
{
#ifdef ENABLE_ALLOCATOR_TEST
	AllocatorTest::Init();
	AllocatorTest::RunTest01();
	AllocatorTest::RunTest02();
	AllocatorTest::RunTest03();
	AllocatorTest::Reset();
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

