#include "Memory\AllocatorOverrides.h"

//void* malloc(size_t size)
//{
//	LOG("Calling malloc(size = %zu)...", size);
//	return engine::BlockAllocator::GetInstance()->Alloc(size);
//}
//
//void free(void* pointer)
//{
//	ASSERT(pointer);
//	LOG("Calling free(pointer = %p)...", pointer);
//	engine::BlockAllocator::GetInstance()->Free(pointer);
//}

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

void* operator new(size_t size, engine::AlignmentType alignment)
{
	LOG("Calling new(size = %zu, alignemnt = %d)...", size, alignment);
	return engine::BlockAllocator::GetInstance()->Alloc(size, alignment);
}

void operator delete(void* pointer, engine::AlignmentType alignment)
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
#endif

void* operator new(size_t size, engine::BlockAllocator* allocator, engine::AlignmentType alignment)
{
	ASSERT(allocator);
	LOG("Calling new(size = %zu, allocator = %p, alignment = %d)", size, allocator, alignment);
	return allocator->Alloc(size, alignment);
}

void operator delete(void* pointer, engine::BlockAllocator* allocator, engine::AlignmentType alignment)
{
	ASSERT(pointer);
	ASSERT(allocator);
	LOG("Calling new(pointer = %p, allocator = %p, alignment = %d)", pointer, allocator, alignment);
	allocator->Free(pointer);
}
