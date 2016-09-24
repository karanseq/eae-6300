// include gameplay elements
#include "MonsterChase.h"

int main(int* argv, char** argc)
{
	// initialize game
	MonsterChase monster_chase;

	while (monster_chase.GetState() != GameStateQuit)
	{
		monster_chase.Update();
	}

	return 0;
}

