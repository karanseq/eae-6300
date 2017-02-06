#ifndef GAME_UTILS_H_
#define GAME_UTILS_H_

// library includes
#include <stdint.h>
#include <stdlib.h>
#include <string>

// engine includes
#include "Math\Vec3D.h"

// game includes
#include "Game\GameTypes.h"

namespace monsterchase {

class GameUtils
{
private:
	GameUtils() = delete;
	GameUtils(const GameUtils& i_copy) = delete;
	GameUtils& operator=(const GameUtils& i_rhs) = delete;
	~GameUtils() = delete;

public:
	inline static engine::math::Vec3D GetRandomVec3D(size_t i_max_x = 1, size_t i_max_y = 1, size_t i_max_z = 1) { return engine::math::Vec3D(static_cast<float>(rand() % i_max_x), static_cast<float>(rand() % i_max_y), static_cast<float>(rand() % i_max_z)); }

	inline static monsterchase::MoveDirections GetRandomDirection() { return static_cast<monsterchase::MoveDirections>(rand() % static_cast<uint8_t>(monsterchase::MoveDirections::kMoveDirectionMax)); }

}; // class GameUtils

} // namespace monsterchase

#endif // GAME_UTILS_H_
