#include "Game\GameData.h"

// library includes

// engine includes
#include "Memory\AllocatorUtil.h"

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
	ClearFileCache();
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

void GameData::AddFileToCache(const std::string& i_file_name, const size_t i_file_size, uint8_t* i_file_content)
{
	// validate inputs
	ASSERT(!i_file_name.empty());
	ASSERT(i_file_size > 0);
	ASSERT(i_file_content);

	// ensure we load a file only once
	ASSERT(file_cache_.count(i_file_name) == 0);

	// cache the file
	file_cache_.insert(std::pair<std::string, FileData>(i_file_name, { i_file_size, i_file_content }));
}

void GameData::RemoveFileFromCache(const std::string& i_file_name)
{
	// validate input
	ASSERT(!i_file_name.empty());

	// ensure we have the file
	ASSERT(file_cache_.count(i_file_name) != 0);

	// deallocate the file
	SAFE_DELETE_ARRAY(file_cache_[i_file_name].file_content);

	// remove the file from the cache
	file_cache_.erase(i_file_name);
}

void GameData::ClearFileCache()
{
	// ensure we clear only once
	ASSERT(!file_cache_.empty());

	for (auto it = file_cache_.begin(); it != file_cache_.end(); ++it)
	{
		SAFE_DELETE_ARRAY(it->second.file_content);
	}
	file_cache_.clear();
}

} // namespace monsterchase
