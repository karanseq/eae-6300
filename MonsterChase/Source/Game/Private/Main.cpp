// engine includes
#include "Memory\BlockAllocator.h"
#include "Memory\AllocatorUtil.h"
#include "Memory\AllocatorOverrides.h"
#include "Math\Vec2D.h"

// game includes
#include "Game\MonsterChase.h"

#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

#ifdef ENABLE_ALLOCATOR_TEST
#include "Tests\AllocatorTest.h"
#include <stdio.h>
#include <conio.h>

bool HeapManager_UnitTest();
#endif // ENABLE_ALLOCATOR_TEST

//#define ENABLE_VECTOR_CONST_TEST

#ifdef ENABLE_VECTOR_CONST_TEST
void TestVectorConstness();
#endif // ENABLE_VECTOR_CONST_TEST

int main(int* argv, char** argc)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	engine::BlockAllocator::RegisterAllocator(default_allocator);

#ifdef ENABLE_VECTOR_CONST_TEST
	TestVectorConstness();
#endif

#ifdef ENABLE_ALLOCATOR_TEST
	printf("Beginning allocator test...\nCheck the Output window for details.\n");
#ifdef BUILD_DEBUG
	HeapManager_UnitTest();
#else
	AllocatorTest::Init(1024*1024);
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
	MonsterChase* monster_chase = new MonsterChase();

	while (monster_chase->GetState() != GameStates::kGameStateQuit)
	{
		monster_chase->Update();
	}

	delete monster_chase;
	monster_chase = nullptr;

	engine::BlockAllocator::DeregisterAllocator(default_allocator);
	engine::BlockAllocator::DestroyDefaultAllocator();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

