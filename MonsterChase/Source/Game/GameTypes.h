#ifndef GAME_TYPES_H_
#define GAME_TYPES_H_

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

#endif // GAME_TYPES_H_
