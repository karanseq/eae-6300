#ifndef ENGINE_ALLOCATOR_UTIL_H_
#define ENGINE_ALLOCATOR_UTIL_H_

#include <stdlib.h>									// for free
//#include "Memory\AllocatorOverrides.h"

#define DEFAULT_BLOCK_SIZE				1024 * 1024
#define DEFAULT_GUARDBAND_SIZE			4
#define DEFAULT_BYTE_ALIGNMENT			4
#define MAX_EXTRA_MEMORY				8

#define GUARDBAND_FILL					0xFD
#define DEAD_FILL						0xDD
#define CLEAN_FILL						0xCD

namespace engine
{
#define SAFE_DELETE(ptr)							do { if (ptr) { delete (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_DELETE_ARRAY(ptr)						do { if (ptr) { delete[] (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_FREE(ptr)								do { if (ptr) { free((ptr)); (ptr) = nullptr; } } while (0)
}

#endif // ENGINE_ALLOCATOR_UTIL_H_
