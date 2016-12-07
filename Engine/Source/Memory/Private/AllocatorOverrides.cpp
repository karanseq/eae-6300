#include "Memory\AllocatorOverrides.h"

//void* malloc(size_t size)
//{
//	LOG("Calling malloc(size = %zu)...", size);
//	return engine::memory::BlockAllocator::GetInstance()->Alloc(size);
//}
//
//void free(void* pointer)
//{
//	ASSERT(pointer);
//	LOG("Calling free(pointer = %p)...", pointer);
//	engine::memory::BlockAllocator::GetInstance()->Free(pointer);
//}

void* operator new(size_t size)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu) on allocator-%d", size, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer)
{
	ASSERT(pointer);

	// get all registered allocators
	engine::memory::BlockAllocator** const allocators = engine::memory::BlockAllocator::GetRegisteredAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_allocators = MAX_ALLOCATORS - 1;
	while (num_allocators > 0)
	{
		if (allocators[num_allocators] && allocators[num_allocators]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on allocator-%d", pointer, allocators[num_allocators]->GetID());
#endif
			return;
		}
		--num_allocators;
	}

	// this means the pointer could not be deleted
	LOG_ERROR("Could not delete(pointer = %p) on any of the %d allocators!", pointer, num_allocators);
}

void* operator new[](size_t size)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu) on allocator-%d", size, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete[](void* pointer)
{
	ASSERT(pointer);

	// get all registered allocators
	engine::memory::BlockAllocator** const allocators = engine::memory::BlockAllocator::GetRegisteredAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_allocators = MAX_ALLOCATORS - 1;
	while (num_allocators > 0)
	{
		if (allocators[num_allocators] && allocators[num_allocators]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on allocator-%d", pointer, allocators[num_allocators]->GetID());
#endif
			return;
		}
		--num_allocators;
	}

	// this means the pointer could not be deleted
	LOG_ERROR("Could not delete(pointer = %p) on any of the %d allocators!", pointer, num_allocators);
}

void* operator new(size_t size, engine::memory::AlignmentType alignment)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, alignemnt = %d) on allocator-%d", size, alignment, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(size, alignment);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, engine::memory::AlignmentType alignment)
{
	ASSERT(pointer);
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, alignemnt = %d) on allocator-%d", pointer, alignment, default_allocator->GetID());
#endif
	default_allocator->Free(pointer);
}

void* operator new(size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p) on allocator-%d", size, allocator, allocator->GetID());
#endif
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, engine::memory::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p) on allocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new[](size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu, allocator = %p) on allocator-%d", size, allocator, allocator->GetID());
#endif
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete[](void* pointer, engine::memory::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete[](pointer = %p, allocator = %p) on allocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new(size_t size, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p, alignment = %d) on allocator-%d", size, allocator, alignment, allocator->GetID());
#endif
	void* pointer = allocator->Alloc(size, alignment);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p, alignment = %d) on allocator-%d", pointer, allocator, alignment, allocator->GetID());
#endif
	allocator->Free(pointer);
}

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t size, const char* file_name, unsigned int line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling new(size = %zu, file_name = %s, line = %d) on allocator-%d", size, file_name, line, default_allocator->GetID());
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, const char* file_name, unsigned int line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling delete(pointer = %p, file_name = %s, line = %d) on allocator-%d", pointer, file_name, line, default_allocator->GetID());
	default_allocator->Free(pointer);
}
#endif
