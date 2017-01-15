#ifndef GAME_TYPES_H_
#define GAME_TYPES_H_

namespace monsterchase {

enum class GameStates
{
	kGameStateNone = 0,
	kGameStateBegin,
	kGameStateInputPlayerName,
	kGameStateInputNumMonsters,
	kGameStateInputMonsterNames,
	kGameStateRunning,
	kGameStateQuit
};

enum class MonsterControllers
{
	kNoMonsterController = 0,
	kSmartMonsterController,
	kSillyMonsterController
};

enum class MoveDirections
{
	kMoveDirectionNone = 0,
	kMoveDirectionLeft,
	kMoveDirectionRight,
	kMoveDirectionUp,
	kMoveDirectionDown,
	kMoveDirectionMax
};

enum class KeyboardKeys
{
	kA = 65,
	kD = 68,
	kM = 77,
	kQ = 81,
	kS = 83,
	kW = 87
};

} // namespace monsterchase

#endif // GAME_TYPES_H_
