// library includes
#include <Windows.h>
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Assert\Assert.h"
#include "GLib.h"
#include "Logger\Logger.h"
#include "Memory\AllocatorUtil.h"

// game includes
#include "Game\MonsterChase.h"

void RunTests();

int WINAPI wWinMain( HINSTANCE i_h_instance, HINSTANCE i_h_prev_instance, LPWSTR i_lp_cmd_line, int i_n_cmd_show )
{
	// initialize allocators
	engine::memory::CreateAllocators();
	
	// initialize GLib
	bool success = GLib::Initialize(i_h_instance, i_n_cmd_show, "MonsterChase", -1, 800, 600);
	ASSERT(success);

	RunTests();
	LOG("\n");

	/*LOG("-------------------- Running MonsterChase --------------------");
	MonsterChase* monster_chase = new MonsterChase();

	while (monster_chase->GetState() != GameStates::kGameStateQuit)
	{
		monster_chase->Update();
	}

	delete monster_chase;
	monster_chase = nullptr;

	LOG("-------------------- Finished MonsterChase --------------------\n\n");*/

	// cleanup GLib
	GLib::Shutdown();

	// cleanup allocators
	engine::memory::DestroyAllocators();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

