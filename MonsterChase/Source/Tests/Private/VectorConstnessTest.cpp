// engine includes
#include "Logger\Logger.h"
#include "Math\Vec2D.h"
#include "Math\Vec3D.h"

void TestVectorConstness()
{
	LOG("-------------------- Running Vec2D, Vec3D Test --------------------");

	const engine::math::Vec2D		v2_a(2.0f, 4.0f);
	const engine::math::Vec2D		v2_b(3.0f, 5.0f);

	const engine::math::Vec2D		v2_c = v2_a + v2_b;

	engine::math::Vec2D				v2_d = -v2_c;

	const engine::math::Vec3D		v3_a(2.0f, 4.0f, 6.0f);
	const engine::math::Vec3D		v3_b(3.0f, 5.0f, 7.0f);

	const engine::math::Vec3D		v3_c = v3_a + v3_b;

	engine::math::Vec3D				v3_d = -v3_c;

	LOG("-------------------- Finished Vec2D, Vec3D Test --------------------");
}
