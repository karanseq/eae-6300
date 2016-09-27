#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

#include <assert.h>

// Define varying levels of verbosity
// At 0, simply print a message
// At 1, print file name and line number ahead of the message
#define DEBUG_LOG_LEVEL 0

#if defined(BUILD_DEBUG)
	#define ASSERT(cond)					assert((cond))

	#if defined(DEBUG_LOG_LEVEL) && (DEBUG_LOG_LEVEL == 1)
		#define LOG_DEBUG(format, ...)			engine::Print(__FUNCTION__, __LINE__, (format), __VA_ARGS__)
	#elif defined(DEBUG_LOG_LEVEL) && (DEBUG_LOG_LEVEL == 0)
		#define LOG_DEBUG(format, ...)			engine::Print((format), __VA_ARGS__)
	#else
		#define LOG_DEBUG(format, ...)			engine::Print((format), __VA_ARGS__)
	#endif
#else
	#define ASSERT(cond)					void(0)
	#define LOG_DEBUG(format, ...)			void(0)
#endif

namespace engine
{
	void Print(const char* format, ...);
	void Print(const char* function_name, const int line_number, const char* format, ...);
} // namespace engine

#endif // ENGINE_LOGGER_H_