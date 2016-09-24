#include "Logger\Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

namespace engine
{
	void Print(const char* format, ...)
	{
		const size_t len_temp = 256;
		char str_temp[len_temp] = "DEBUG: ";

#if defined(DEBUG_LOG_LEVEL) && (DEBUG_LOG_LEVEL == 1)
		sprintf_s(str_temp, "DEBUG-%s-%d:", __FUNCTION__, __LINE__);
#endif

		strcat_s(str_temp, format);
		strcat_s(str_temp, "\n");

		const size_t len_output = len_temp + 32;
		char str_output[len_output] = { 0 };

		va_list args;
		va_start(args, format);
		vsprintf_s(str_output, len_output, str_temp, args);
		va_end(args);

		OutputDebugStringA(str_output);
	}

} // namespace engine