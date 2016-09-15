// include libraries
#include <stdio.h>

// include gameplay elements
#include "MonsterChase.h"

#include "Vec2D.h"

int main(int* argv, char** argc)
{
	// initialize game
	MonsterChase monster_chase;

	while (monster_chase.GetState() != GameStateQuit)
	{
		monster_chase.Update();
	}

	//engine::Vec2D a(1.0f, -2.0f), b, c;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");
	//b = a;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");
	//c = a + b;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");
	//printf("a & b are %s\n", (a == b ? "equal" : "not equal"));
	//printf("a & c are %s\n", (a != c ? "not equal" : "equal"));
	//printf("----------------------------\n");
	//a -= c;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");
	//b += a;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");
	//b = a - c;
	//printf("a:%f,%f  b:%f,%f  c:%f,%f\n", a.x(), a.y(), b.x(), b.y(), c.x(), c.y());
	//printf("----------------------------\n");

	//getchar();

	return 0;
}

