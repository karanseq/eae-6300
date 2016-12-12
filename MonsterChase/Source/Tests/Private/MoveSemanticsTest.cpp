// library includes
#include <vector>

// engine includes
#include "Logger\Logger.h"

// game includes
#include "Game\Monster.h"
#include "Game\MonsterChase.h"
#include "Game\Player.h"
#include "Game\PlayerController.h"
#include "Game\SillyMonsterController.h"
#include "Game\SmartMonsterController.h"

void TestMoveSemantics()
{
	LOG("-------------------- Running Move Semantics Test --------------------");

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

	LOG("-------------------- Player --------------------");
	Player player_a("AAA");
	Player player_b("BBB");
	// test move assignment operator
	player_b = std::move(player_a);
	LOG("Player-A's name:%s\t\tPlayer-B's name:%s", player_a.GetIdentity()->GetName(), player_b.GetIdentity()->GetName());

	LOG("-------------------- Monster --------------------");
	Monster monster_silly(MonsterControllers::kSillyMonsterController, "SILLY");
	Monster monster_smart(MonsterControllers::kSmartMonsterController, "SMART");
	// test move assignment operator
	monster_smart = std::move(monster_silly);
	LOG("Silly-Monster's name:%s\t\tSmart-Monster's name:%s", monster_silly.GetIdentity()->GetName(), monster_smart.GetIdentity()->GetName());
	
	LOG("-------------------- Testing Move Constructors --------------------");
	const size_t num_objects = 3;
	LOG("-------------------- SillyMonsterController --------------------");
	std::vector<SillyMonsterController> silly_controllers;
	silly_controllers.reserve(num_objects);

	for (uint8_t i = 0; i < num_objects; ++i)
	{
		silly_controllers.push_back(SillyMonsterController());
	}
	silly_controllers.clear();

	LOG("-------------------- SmartMonsterController --------------------");
	std::vector<SmartMonsterController> smart_controllers;
	smart_controllers.reserve(num_objects);

	for (uint8_t i = 0; i < num_objects; ++i)
	{
		smart_controllers.push_back(SmartMonsterController());
	}
	smart_controllers.clear();

	LOG("-------------------- PlayerController --------------------");
	std::vector<PlayerController> player_controllers;
	player_controllers.reserve(num_objects);

	for (uint8_t i = 0; i < num_objects; ++i)
	{
		player_controllers.push_back(PlayerController());
	}
	player_controllers.clear();

	LOG("-------------------- Player --------------------");
	std::vector<Player> players;
	players.reserve(num_objects);

	for (uint8_t i = 0; i < num_objects; ++i)
	{
		players.push_back(Player(""));
	}
	players.clear();

	LOG("-------------------- Monster --------------------");
	std::vector<Monster> monsters;
	monsters.reserve(num_objects);

	for (uint8_t i = 0; i < num_objects; ++i)
	{
		monsters.push_back(Monster(i % 2 ? MonsterControllers::kSillyMonsterController : MonsterControllers::kSmartMonsterController, ""));
	}
	monsters.clear();

	LOG("-------------------- Finished Move Semantics Test --------------------");
}
