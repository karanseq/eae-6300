// library includes
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// game includes
#include "Game\MonsterChase.h"

void RunTests();

int main(int* argv, char** argc)
{
	// initialize the default allocator
	engine::memory::BlockAllocator::CreateDefaultAllocator();
	
	RunTests();

	// initialize game
	MonsterChase* monster_chase = new MonsterChase();

	while (monster_chase->GetState() != GameStates::kGameStateQuit)
	{
		monster_chase->Update();
	}

	delete monster_chase;
	monster_chase = nullptr;

	// destroy the default allocator
	engine::memory::BlockAllocator::DestroyDefaultAllocator();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

