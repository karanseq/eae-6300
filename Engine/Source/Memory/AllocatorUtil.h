#ifndef ENGINE_ALLOCATOR_UTIL_H_
#define ENGINE_ALLOCATOR_UTIL_H_

#include <stdlib.h>									// for free
//#include "Memory\AllocatorOverrides.h"

namespace engine
{
#define SAFE_DELETE(ptr)							do { if (ptr) { delete (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_DELETE_ARRAY(ptr)						do { if (ptr) { delete[] (ptr); (ptr) = nullptr; } } while (0)
#define SAFE_FREE(ptr)								do { if (ptr) { free((ptr)); (ptr) = nullptr; } } while (0)
}

#endif // ENGINE_ALLOCATOR_UTIL_H_
