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
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu) on allocator-%d", size, default_allocator->GetID());
#endif
	return default_allocator->Alloc(size);
}

void operator delete(void* pointer)
{
	ASSERT(pointer);
	
	bool success = false;
	
	// loop the known allocators list
	const engine::BANode* allocator_node = engine::BlockAllocator::GetKnownAllocators();
	while (allocator_node != nullptr)
	{
		engine::BlockAllocator* allocator = allocator_node->block_allocator_;
		if (allocator && allocator->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Calling delete(pointer = %p) on allocator-%d", pointer, allocator->GetID());
#endif
			success = true;
			break;
		}

		allocator_node = allocator_node->next_;
	}

	// assert if we couldn't delete this pointer
	ASSERT(success);
}

void* operator new[](size_t size)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu) on allocator-%d", size, default_allocator->GetID());
#endif
	return default_allocator->Alloc(size);
}

void operator delete[](void* pointer)
{
	ASSERT(pointer);
	
	bool success = false;

	// loop the known allocators list
	const engine::BANode* allocator_node = engine::BlockAllocator::GetKnownAllocators();
	while (allocator_node != nullptr)
	{
		engine::BlockAllocator* allocator = allocator_node->block_allocator_;
		if (allocator && allocator->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Calling delete[](pointer = %p) on allocator-%d", pointer, allocator->GetID());
#endif
			success = true;
			break;
		}

		allocator_node = allocator_node->next_;
	}

	// assert if we couldn't delete this pointer
	ASSERT(success);
}

void* operator new(size_t size, engine::AlignmentType alignment)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, alignemnt = %d) on allocator-%d", size, alignment, default_allocator->GetID());
#endif
	return default_allocator->Alloc(size, alignment);
}

void operator delete(void* pointer, engine::AlignmentType alignment)
{
	ASSERT(pointer);
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, alignemnt = %d) on allocator-%d", pointer, alignment, default_allocator->GetID());
#endif
	default_allocator->Free(pointer);
}

void* operator new(size_t size, engine::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p) on allocator-%d", size, allocator, allocator->GetID());
#endif
	return allocator->Alloc(size);
}

void operator delete(void* pointer, engine::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p) on allocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new[](size_t size, engine::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu, allocator = %p) on allocator-%d", size, allocator, allocator->GetID());
#endif
	return allocator->Alloc(size);
}

void operator delete[](void* pointer, engine::BlockAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete[](pointer = %p, allocator = %p) on allocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new(size_t size, engine::BlockAllocator* allocator, engine::AlignmentType alignment)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p, alignment = %d) on allocator-%d", size, allocator, alignment, allocator->GetID());
#endif
	return allocator->Alloc(size, alignment);
}

void operator delete(void* pointer, engine::BlockAllocator* allocator, engine::AlignmentType alignment)
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
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling new(size = %zu, file_name = %s, line = %d) on allocator-%d", size, file_name, line, default_allocator->GetID());
	return default_allocator->Alloc(size);
}

void operator delete(void* pointer, const char* file_name, unsigned int line)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling delete(pointer = %p, file_name = %s, line = %d) on allocator-%d", pointer, file_name, line, default_allocator->GetID());
	default_allocator->Free(pointer);
}
#endif
