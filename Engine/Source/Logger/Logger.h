#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

#include <assert.h>

#if defined(BUILD_DEBUG)
	#define ASSERT(cond)					assert((cond))
	#define LOG_DEBUG(format, ...)			engine::Print((format), __VA_ARGS__)
#else
	#define ASSERT(cond)					void(0)
	#define LOG_DEBUG(format, ...)			void(0)
#endif

namespace engine
{
	void Print(const char* format, ...);
} // namespace engine

#endif // ENGINE_LOGGER_H_