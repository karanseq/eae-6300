#include "Collider.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace physics {

inline Collider* Collider::Get()
{
    return Collider::instance_;
}

} // namespace physics
} // namespace engine
