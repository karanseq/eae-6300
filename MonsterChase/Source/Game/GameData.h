#ifndef GAME_DATA_H_
#define GAME_DATA_H_

// library includes
#include <map>
#include <string>

// engine includes
#include "Assert\Assert.h"

namespace monsterchase {

struct FileData {
	size_t		file_size;
	uint8_t*	file_content;
};

class GameData
{
private:
	GameData();
	~GameData();
	static GameData* instance_;

	// disable copy constructor & copy assignment operator
	GameData(const GameData& i_copy) = delete;
	GameData& operator=(const GameData& i_copy) = delete;

	void AddFileToCache(const std::string& i_file_name, const size_t i_file_size, uint8_t* i_file_content);
	void RemoveFileFromCache(const std::string& i_file_name);
	void ClearFileCache();
	inline bool IsFileCached(const std::string& i_file_name)					{ ASSERT(!i_file_name.empty()); return (file_cache_.find(i_file_name) != file_cache_.end()); }

public:
	static GameData* Create();
	static void Destroy();
	static inline GameData* GetInstance() { return instance_; }

	inline const std::map<std::string, FileData>& GetFileCache()				{ return file_cache_; }

	static const char*							PLAYER_TEXTURE_NAME;
	static const char*							SILLY_MONSTER_TEXTURE_NAME;
	static const char*							SMART_MONSTER_TEXTURE_NAME;

private:
	std::map<std::string, FileData>				file_cache_;

	friend class GameUtils;
}; // class GameData

} // namespace monsterchase

#endif // GAME_DATA_H_
