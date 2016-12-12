#ifndef GAME_UTILS_H_
#define GAME_UTILS_H_

// library includes
#include <stdlib.h>

// engine includes
#include "Math\Vec3D.h"

// game includes
#include "Game\GameTypes.h"

class GameUtils
{
private:
	GameUtils() {}
	GameUtils(const GameUtils& copy);
	GameUtils& operator=(const GameUtils& rhs);
	~GameUtils() {}

public:
	inline static engine::math::Vec3D GetRandomVec3D(size_t max_x = 1, size_t max_y = 1, size_t max_z = 1) { return engine::math::Vec3D(static_cast<float>(rand() % max_x), static_cast<float>(rand() % max_y), static_cast<float>(rand() % max_z)); }

	inline static MoveDirections GetRandomDirection() { return static_cast<MoveDirections>(rand() % static_cast<uint8_t>(MoveDirections::kMoveDirectionMax)); }

}; // class GameUtils

#endif // GAME_UTILS_H_
