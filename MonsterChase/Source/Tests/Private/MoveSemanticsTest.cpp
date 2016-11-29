// library includes
#include <vector>

// engine includes
#include "Logger\Logger.h"

// game includes
#include "Game\MonsterChase.h"
#include "Game\SillyMonsterController.h"
#include "Game\SmartMonsterController.h"
#include "Game\PlayerController.h"

void TestMoveSemantics()
{
	MonsterChase monster_chase;

	LOG("-------------------- Testing Move Assignment Operators --------------------");
	LOG("-------------------- SillyMonsterController --------------------");
	SillyMonsterController silly_controller_a;
	SillyMonsterController silly_controller_b;
	// test move assignment operator
	silly_controller_b = std::move(silly_controller_a);

	LOG("-------------------- SmartMonsterController --------------------");
	SmartMonsterController smart_controller_a;
	SmartMonsterController smart_controller_b;
	// test move assignment operator
	smart_controller_b = std::move(smart_controller_a);

	LOG("-------------------- PlayerController --------------------");
	PlayerController player_controller_a;
	PlayerController player_controller_b;
	// test move assignment operator
	player_controller_b = std::move(player_controller_a);
	
	LOG("-------------------- Testing Move Constructors --------------------");
	const size_t num_controllers = 5;
	LOG("-------------------- SillyMonsterController --------------------");
	std::vector<SillyMonsterController> silly_controllers;
	silly_controllers.reserve(num_controllers);

	for (uint8_t i = 0; i < num_controllers; ++i)
	{
		silly_controllers.push_back(SillyMonsterController());
	}
	silly_controllers.clear();

	LOG("-------------------- SmartMonsterController --------------------");
	std::vector<SmartMonsterController> smart_controllers;
	smart_controllers.reserve(num_controllers);

	for (uint8_t i = 0; i < num_controllers; ++i)
	{
		smart_controllers.push_back(SmartMonsterController());
	}
	smart_controllers.clear();

	LOG("-------------------- PlayerController --------------------");
	std::vector<PlayerController> player_controllers;
	player_controllers.reserve(num_controllers);

	for (uint8_t i = 0; i < num_controllers; ++i)
	{
		player_controllers.push_back(PlayerController());
	}
	player_controllers.clear();
}
