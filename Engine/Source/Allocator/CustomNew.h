#ifndef CUSTOM_NEW_H_
#define CUSTOM_NEW_H_

#include "Allocator\BlockAllocator.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"

void* operator new(size_t size)
{
	LOG("Calling new(size = %zu)...", size);
	return engine::BlockAllocator::GetInstance()->Alloc(size);
}

void operator delete(void* pointer)
{
	ASSERT(pointer);
	LOG("Calling delete(pointer = %p)...", pointer);
	engine::BlockAllocator::GetInstance()->Free(pointer);
}

void* operator new[](size_t size)
{
	LOG("Calling new[](size = %zu)...", size);
	return engine::BlockAllocator::GetInstance()->Alloc(size);
}

void operator delete[](void* pointer)
{
	ASSERT(pointer);
	LOG("Calling delete[](pointer = %p)...", pointer);
	engine::BlockAllocator::GetInstance()->Free(pointer);
}

enum AlignmentType
{
	ALIGNMENT_DEFAULT = 4,
	ALIGNMENT_8 = 8,
	ALIGNMENT_16 = 16,
	ALIGNMENT_32 = 32,
	ALIGNMENT_64 = 64
};

void* operator new(size_t size, AlignmentType alignment)
{
	LOG("Calling new(size = %zu, alignemnt = %d)...", size, alignment);
	return engine::BlockAllocator::GetInstance()->Alloc(size, alignment);
}

void operator delete(void* pointer, AlignmentType alignment)
{
	ASSERT(pointer);
	LOG("Calling delete(pointer = %p, alignemnt = %d)...", pointer, alignment);
	engine::BlockAllocator::GetInstance()->Free(pointer);
}

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)

void* operator new(size_t size, const char* file_name, unsigned int line)
{
	LOG("Calling new(size = %zu, file_name = %s, line = %d)", size, file_name, line);
	return engine::BlockAllocator::GetInstance()->Alloc(size);
}

void operator delete(void* pointer, const char* file_name, unsigned int line)
{
	LOG("Calling delete(pointer = %p, file_name = %s, line = %d)", pointer, file_name, line);
}
#else
#define TRACK_NEW
#endif

void* operator new(size_t size, engine::BlockAllocator* allocator, AlignmentType alignment)
{
	ASSERT(allocator);
	LOG("Calling new(size = %zu, allocator = %p, alignment = %d)", size, allocator, alignment);
	return allocator->Alloc(size, alignment);
}

void operator delete(void* pointer, engine::BlockAllocator* allocator, AlignmentType alignment)
{
	ASSERT(pointer);
	ASSERT(allocator);
	LOG("Calling new(pointer = %p, allocator = %p, alignment = %d)", pointer, allocator, alignment);
	allocator->Free(pointer);
}

#endif // CUSTOM_NEW_H_