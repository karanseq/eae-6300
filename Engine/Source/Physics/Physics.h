#ifndef PHYSICS_H_
#define PHYSICS_H_

namespace engine {
namespace physics {

/*
	Physics
	- A class that updates physics objects
*/

class Physics
{
private:
	Physics();
	~Physics();
	static Physics* instance_;

	Physics(const Physics& i_copy);
	Physics& operator=(const Physics& i_copy);

public:
	static Physics* Create();
	static void Destroy();
	static inline Physics* Get();

	void Run(float dt);
};

} // namespace physics
} // namespace engine

#include "Physics-inl.h"

#endif // PHYSICS_H_
