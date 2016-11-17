#ifndef CUSTOM_NEW_H_
#define CUSTOM_NEW_H_

#include "Memory\BlockAllocator.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine
{
	enum AlignmentType
	{
		ALIGNMENT_DEFAULT = 4,
		ALIGNMENT_8 = 8,
		ALIGNMENT_16 = 16,
		ALIGNMENT_32 = 32,
		ALIGNMENT_64 = 64
	};

} // namespace engine

//void* malloc(size_t size);
//void free(void* pointer);

void* operator new(size_t size);
void operator delete(void* pointer);

void* operator new[](size_t size);
void operator delete[](void* pointer);

void* operator new(size_t size, engine::AlignmentType alignment);
void operator delete(void* pointer, engine::AlignmentType alignment);

void* operator new(size_t size, engine::BlockAllocator* allocator);
void operator delete(void* pointer, engine::BlockAllocator* allocator);

void* operator new[](size_t size, engine::BlockAllocator* allocator);
void operator delete[](void* pointer, engine::BlockAllocator* allocator);

void* operator new(size_t size, engine::BlockAllocator* allocator, engine::AlignmentType alignment);
void operator delete(void* pointer, engine::BlockAllocator* allocator, engine::AlignmentType alignment);

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t size, const char* file_name, unsigned int line);
void operator delete(void* pointer, const char* file_name, unsigned int line);
#else
#define TRACK_NEW
#endif

#endif // CUSTOM_NEW_H_