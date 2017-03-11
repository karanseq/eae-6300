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

// game includes
#include "Game\Game.h"

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
		bool success = GLib::Initialize(i_h_instance, i_n_cmd_show, "Game", -1, game::Game::SCREEN_WIDTH, game::Game::SCREEN_HEIGHT);
		ASSERT(success);

#ifdef ENABLE_TESTS
		RunTests();
		LOG("\n");
#endif // ENABLE_TESTS

		// init game
		/*if (game::StartUp())
		{
			game::Game* game = game::Game::GetInstance();
			engine::Run();
		}

		// cleanup game
		game::Shutdown();*/

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

