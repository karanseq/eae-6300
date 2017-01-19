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
#include "Timer\TimerUtil.h"

// game includes
#include "Game\MonsterChase.h"

#ifdef ENABLE_TESTS
void RunTests();
#endif // ENABLE_TESTS

int WINAPI wWinMain( HINSTANCE i_h_instance, HINSTANCE i_h_prev_instance, LPWSTR i_lp_cmd_line, int i_n_cmd_show )
{
	// initialize allocators
	engine::memory::CreateAllocators();
	
	// initialize GLib
	bool success = GLib::Initialize(i_h_instance, i_n_cmd_show, "MonsterChase", -1, 1280, 800);
	ASSERT(success);

#ifdef ENABLE_TESTS
	RunTests();
	LOG("\n");
#endif // ENABLE_TESTS

	LOG("-------------------- Running MonsterChase --------------------");
	monsterchase::MonsterChase* monster_chase = monsterchase::MonsterChase::Create();
	monster_chase->Init();

	while (monster_chase->GetState() != monsterchase::GameStates::kGameStateQuit)
	{
		monster_chase->Update();
	}

	monsterchase::MonsterChase::Destroy();

	LOG("-------------------- Finished MonsterChase --------------------\n\n");

	// cleanup GLib
	GLib::Shutdown();

	// cleanup allocators
	engine::memory::DestroyAllocators();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

