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
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer)
{
	ASSERT(pointer);

	// calculate the address of the descriptor from the user's pointer
#ifdef BUILD_DEBUG
	uint8_t* descriptor_address = static_cast<uint8_t*>(pointer) - DEFAULT_GUARDBAND_SIZE - engine::BlockAllocator::GetSizeOfBD();
#else
	uint8_t* descriptor_address = static_cast<uint8_t*>(pointer) - engine::BlockAllocator::GetSizeOfBD();
#endif
	// cast to get a descriptor
	engine::BD* descriptor = reinterpret_cast<engine::BD*>(descriptor_address);
	ASSERT(descriptor);
	
	// get the allocator from the descriptor
	engine::BlockAllocator* allocator = descriptor->allocator_;
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p) on allocator-%d", pointer, allocator->GetID());
#endif

	// call free on the pointer
	allocator->Free(pointer);
}

void* operator new[](size_t size)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
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

	// calculate the address of the descriptor from the user's pointer
#ifdef BUILD_DEBUG
	uint8_t* descriptor_address = static_cast<uint8_t*>(pointer) - DEFAULT_GUARDBAND_SIZE - engine::BlockAllocator::GetSizeOfBD();
#else
	uint8_t* descriptor_address = static_cast<uint8_t*>(pointer) - engine::BlockAllocator::GetSizeOfBD();
#endif
	// cast to get a descriptor
	engine::BD* descriptor = reinterpret_cast<engine::BD*>(descriptor_address);
	ASSERT(descriptor);

	// get the allocator from the descriptor
	engine::BlockAllocator* allocator = descriptor->allocator_;
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p) on allocator-%d", pointer, allocator->GetID());
#endif

	// call free on the pointer
	allocator->Free(pointer);
}

void* operator new(size_t size, engine::AlignmentType alignment)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, alignemnt = %d) on allocator-%d", size, alignment, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(size, alignment);
	ASSERT(pointer);
	return pointer;
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
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
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
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
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
	void* pointer = allocator->Alloc(size, alignment);
	ASSERT(pointer);
	return pointer;
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
	void* pointer = default_allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, const char* file_name, unsigned int line)
{
	engine::BlockAllocator* default_allocator = engine::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling delete(pointer = %p, file_name = %s, line = %d) on allocator-%d", pointer, file_name, line, default_allocator->GetID());
	default_allocator->Free(pointer);
}
#endif
