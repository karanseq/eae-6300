#include "Game\GameData.h"

// library includes

// engine includes
#include "Common\HelperMacros.h"

namespace monsterchase {

// static member initialization
GameData* GameData::instance_ = nullptr;

const char* GameData::PLAYER_TEXTURE_NAME = "Data\\clown.dds";
const char* GameData::SILLY_MONSTER_TEXTURE_NAME = "Data\\enemy_01.dds";
const char* GameData::SMART_MONSTER_TEXTURE_NAME = "Data\\enemy_05.dds";

GameData::GameData()
{}

GameData::~GameData()
{
	
}

GameData* GameData::Create()
{
	if (!GameData::instance_)
	{
		GameData::instance_ = new GameData();
	}
	return GameData::instance_;
}

void GameData::Destroy()
{
	SAFE_DELETE(GameData::instance_);
}

} // namespace monsterchase
