#ifndef UPDATER_H_
#define UPDATER_H_

// library includes
#include <vector>

namespace engine {
namespace time {

// forward declarations
class InterfaceTickable;

class Updater
{
private:
	Updater();
	~Updater();
	static Updater* instance_;

	Updater(const Updater& i_copy);
	Updater& operator=(const Updater& i_copy);

public:
	static Updater* Create();
	static void Destroy();
	static inline Updater* Get();

	// add/remove a tickable
	inline void AddTickable(InterfaceTickable* i_tickable);
	inline void RemoveTickable(InterfaceTickable* i_tickable);

	// the tick
	void Run(float dt);

private:
	size_t													num_tickables_;
	std::vector<InterfaceTickable*>							tickables_;

}; // class Updater

} // namespace time
} // namespace engine

#include "Updater-inl.h"

#endif // UPDATER_H_