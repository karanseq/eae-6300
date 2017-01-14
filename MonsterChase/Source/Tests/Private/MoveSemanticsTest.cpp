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

	monsterchase::MonsterChase* monster_chase = monsterchase::MonsterChase::Create();

	// create local scope so objects are deleted before MonsterChase
	{
		LOG("-------------------- Testing Move Assignment Operators --------------------");
		LOG("-------------------- SillyMonsterController --------------------");
		monsterchase::SillyMonsterController silly_controller_a;
		monsterchase::SillyMonsterController silly_controller_b;
		// test move assignment operator
		silly_controller_b = std::move(silly_controller_a);

		LOG("-------------------- SmartMonsterController --------------------");
		monsterchase::SmartMonsterController smart_controller_a;
		monsterchase::SmartMonsterController smart_controller_b;
		// test move assignment operator
		smart_controller_b = std::move(smart_controller_a);

		LOG("-------------------- PlayerController --------------------");
		monsterchase::PlayerController player_controller_a;
		monsterchase::PlayerController player_controller_b;
		// test move assignment operator
		player_controller_b = std::move(player_controller_a);

		LOG("-------------------- Player --------------------");
		monsterchase::Player player_a("AAA");
		monsterchase::Player player_b("BBB");
		// test move assignment operator
		player_b = std::move(player_a);
		LOG("Player-A's name:%s\t\tPlayer-B's name:%s", player_a.GetIdentity()->GetName(), player_b.GetIdentity()->GetName());

		LOG("-------------------- Monster --------------------");
		monsterchase::Monster monster_silly(MonsterControllers::kSillyMonsterController, "SILLY");
		monsterchase::Monster monster_smart(MonsterControllers::kSmartMonsterController, "SMART");
		// test move assignment operator
		monster_smart = std::move(monster_silly);
		LOG("Silly-Monster's name:%s\t\tSmart-Monster's name:%s", monster_silly.GetIdentity()->GetName(), monster_smart.GetIdentity()->GetName());
	
		LOG("-------------------- Testing Move Constructors --------------------");
		const size_t num_objects = 3;
		LOG("-------------------- SillyMonsterController --------------------");
		std::vector<monsterchase::SillyMonsterController> silly_controllers;
		silly_controllers.reserve(num_objects);

		for (uint8_t i = 0; i < num_objects; ++i)
		{
			silly_controllers.push_back(monsterchase::SillyMonsterController());
		}
		silly_controllers.clear();

		LOG("-------------------- SmartMonsterController --------------------");
		std::vector<monsterchase::SmartMonsterController> smart_controllers;
		smart_controllers.reserve(num_objects);

		for (uint8_t i = 0; i < num_objects; ++i)
		{
			smart_controllers.push_back(monsterchase::SmartMonsterController());
		}
		smart_controllers.clear();

		LOG("-------------------- PlayerController --------------------");
		std::vector<monsterchase::PlayerController> player_controllers;
		player_controllers.reserve(num_objects);

		for (uint8_t i = 0; i < num_objects; ++i)
		{
			player_controllers.push_back(monsterchase::PlayerController());
		}
		player_controllers.clear();

		LOG("-------------------- Player --------------------");
		std::vector<monsterchase::Player> players;
		players.reserve(num_objects);

		for (uint8_t i = 0; i < num_objects; ++i)
		{
			players.push_back(monsterchase::Player(""));
		}
		players.clear();

		LOG("-------------------- Monster --------------------");
		std::vector<monsterchase::Monster> monsters;
		monsters.reserve(num_objects);

		for (uint8_t i = 0; i < num_objects; ++i)
		{
			monsters.push_back(monsterchase::Monster(i % 2 ? MonsterControllers::kSillyMonsterController : MonsterControllers::kSmartMonsterController, ""));
		}
		monsters.clear();
	}

	monsterchase::MonsterChase::Destroy();

	LOG("-------------------- Finished Move Semantics Test --------------------");
}
