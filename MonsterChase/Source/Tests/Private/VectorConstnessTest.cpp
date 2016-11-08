#include "Math\Vec2D.h"

void TestVectorConstness()
{
	const engine::Vec2D a(2.0f, 4.0f);
	const engine::Vec2D b(3.0f, 5.0f);

	const engine::Vec2D c = a + b;

	engine::Vec2D d = -c;
}
