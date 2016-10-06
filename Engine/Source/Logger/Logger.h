#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

#include <assert.h>

/*
	A simple logging & assert utility.
	Features:
	- Supports a simple assert function in debug mode at all verbosity levels
	- Supports a simple log function in both debug and release modes at various verbosity levels (see Variables section)
	- Supports a simple verbose function that adds file name and line number to a regular log in debug mode at max verbosity (see Variables section)

	Variables:
	- BUILD_DEBUG: Define this only when we're in debug mode.
	- VERBOSITY_LEVEL
		- In RELEASE mode, set VERBOSITY_LEVEL to 1 to enable logs, 0 to disable all logs
			VERBOSITY_LEVEL = 0
				- Assert, log & error are disabled
			VERBOSITY_LEVEL = 1
				- Assert is disabled, log & error are enabled

		- In DEBUG mode,
			VERBOSITY_LEVEL = 0 or 1
				- Assert, log & error are enabled, verbose is disabled
			VERBOSITY_LEVEL = 2
				- Assert, log, error & verbose are enabled
*/

#if defined(BUILD_DEBUG)
	// in debug mode, enable assert
	#define ASSERT(cond)					assert((cond))

	// in debug mode, enable verbose & log when level is 2
	#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 1)
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
		#define VERBOSE(format, ...)		engine::Print(__FUNCTION__, __LINE__, (format), __VA_ARGS__)
	// in debug mode, enable log when level is 1	
	#elif defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 0)
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
		#define VERBOSE(format, ...)		void(0)
	#else
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
		#define VERBOSE(format, ...)		void(0)
	#endif
#else
	// in release mode, disable assert and verbose
	#define ASSERT(cond)					void(0)
	#define VERBOSE(format, ...)			void(0)

	// in release mode, enable log & error when level is 1
	#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 0)
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
	#else
		#define LOG(format, ...)			void(0)
		#define LOG_ERROR(format, ...)		void(0)
	#endif
#endif

namespace engine
{
	void Print(const char* type, const char* format, ...);
#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 1)
	void Print(const char* function_name, const int line_number, const char* format, ...);
#endif
} // namespace engine

#endif // ENGINE_LOGGER_H_