#ifndef GAME_DATA_H_
#define GAME_DATA_H_

// library includes
#include <map>
#include <string>

// engine includes
#include "Assert\Assert.h"

namespace game {

struct FileData {
	size_t		file_size;
	uint8_t*	file_content;
};

class GameData
{
private:
	GameData() = delete;
	~GameData() = delete;

	// disable copy constructor & copy assignment operator
	GameData(const GameData& i_copy) = delete;
	GameData& operator=(const GameData& i_copy) = delete;

public:
	static const char*							PLAYER_TEXTURE_NAME;
	static const char*							SILLY_MONSTER_TEXTURE_NAME;
	static const char*							SMART_MONSTER_TEXTURE_NAME;
	static const char*							PLAYER_LUA_FILE_NAME;
	static const char*							MONSTERS_LUA_FILE_NAME;
	static const char*							MONSTER_LUA_FILE_NAME;
    static const char*                          BLOCK_LUA_FILE_NAME;

}; // class GameData

} // namespace monsterchase

#endif // GAME_DATA_H_
