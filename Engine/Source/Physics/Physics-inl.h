#include "Physics.h"

namespace engine {
namespace physics {

inline Physics* Physics::Get()
{
	return Physics::instance_;
}

} // namespace physics
} // namespace engine