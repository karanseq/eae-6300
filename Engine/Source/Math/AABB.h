#ifndef AABB_H_
#define AABB_H_

// engine includes
#include "Vec2D.h"

namespace engine {
namespace math {

struct AABB
{
    Vec2D                   center;
    Vec2D                   extents;

    static const AABB       ZERO;
};

} // namespace math
} // namespace engine

#endif // AABB_H_