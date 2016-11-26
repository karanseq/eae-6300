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
	inline static engine::math::Vec3D GetRandomVec3D(int max_x = 1, int max_y = 1, int max_z = 1) { return engine::math::Vec3D(static_cast<float>(rand() % max_x), static_cast<float>(rand() % max_y), static_cast<float>(rand() % max_z)); }

	inline static MoveDirections GetRandomDirection() { return static_cast<MoveDirections>(rand() % static_cast<int>(MoveDirections::kMoveDirectionMax)); }

}; // class GameUtils

#endif // GAME_UTILS_H_
