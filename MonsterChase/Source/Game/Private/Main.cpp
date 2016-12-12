// library includes
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Logger\Logger.h"

// game includes
#include "Game\MonsterChase.h"

void RunTests();

int main(int* argv, char** argc)
{
	engine::memory::CreateAllocators();
	
	RunTests();
	LOG("\n");

	LOG("-------------------- Running MonsterChase --------------------");
	// initialize game
	MonsterChase* monster_chase = new MonsterChase();

	while (monster_chase->GetState() != GameStates::kGameStateQuit)
	{
		monster_chase->Update();
	}

	delete monster_chase;
	monster_chase = nullptr;

	LOG("-------------------- Finished MonsterChase --------------------\n\n");

	engine::memory::DestroyAllocators();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

