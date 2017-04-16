// library includes
#include <stdio.h>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"
#include "Math\MathUtil.h"
#include "Math\Vec3D.h"
#include "Math\Vec4D.h"
#include "Math\Vec3D-SSE.h"
#include "Math\Vec4D-SSE.h"

void TestFastMath()
{
    LOG("-------------------- Running Fast Math Test --------------------");

    using namespace engine::math;

    //~==================================================================
    // Vec3D
    optimized::Vec3D vec01(1.0f, 2.0f, 3.0f);
    vec01 *= 2.0f;

    float length_squared_01 = vec01.LengthSquared();
    ASSERT(engine::math::FuzzyEquals(length_squared_01, 56.0f));

    optimized::Vec3D vec02(2.0f, 4.0f, 6.0f);
    ASSERT(vec01 == vec02);

    optimized::Vec3D vec03(-1.0f, 11.0f, -22.0f);

    Vec3D slow01(2.0f, 4.0f, 6.0f);
    Vec3D slow02(-1.0f, 11.0f, -22.0f);

    ASSERT(FuzzyEquals(DotProduct(vec02, vec03), DotProduct(slow01, slow02)));

    optimized::Vec3D fast_cross = CrossProduct(vec02, vec03);
    Vec3D slow_cross = CrossProduct(slow01, slow02);

    ASSERT(fast_cross == optimized::Vec3D(slow_cross.x(), slow_cross.y(), slow_cross.z()));

    //~==================================================================
    // Vec4D
    optimized::Vec4D vec11(1.0f, 2.0f, 3.0f, 4.0f);
    vec11 *= 2.0f;

    float length_squared_11 = vec11.LengthSquared();
    ASSERT(engine::math::FuzzyEquals(length_squared_11, 120.0f));

    optimized::Vec4D vec12(2.0f, 4.0f, 6.0f, 8.0f);
    ASSERT(vec11 == vec12);

    LOG("-------------------- Finished Fast Math Test --------------------");
}

