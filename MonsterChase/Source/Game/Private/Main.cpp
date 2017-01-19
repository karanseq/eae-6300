// library includes
#include <Windows.h>
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Assert\Assert.h"
#include "Common\Engine.h"
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
	// init engine
	if (engine::StartUp())
	{
		// TODO: Is this the right place to do this?
		// initialize GLib
		bool success = GLib::Initialize(i_h_instance, i_n_cmd_show, "MonsterChase", -1, 1280, 800);
		ASSERT(success);

#ifdef ENABLE_TESTS
		RunTests();
		LOG("\n");
#endif // ENABLE_TESTS

		// init game
		if (monsterchase::StartUp())
		{
			monsterchase::MonsterChase* monster_chase = monsterchase::MonsterChase::GetInstance();

			// TODO: Replace this with a blocking function
			while (monster_chase->GetState() != monsterchase::GameStates::kGameStateQuit)
			{
				monster_chase->Update();
			}
		}

		// cleanup game
		monsterchase::Shutdown();

		// TODO: Is this the right place to do this?
		// cleanup GLib
		GLib::Shutdown();
	}

	// cleanup
	engine::Shutdown();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}

