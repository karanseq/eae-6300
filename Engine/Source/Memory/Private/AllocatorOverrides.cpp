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

void* operator new(size_t i_size)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu) on allocator-%d", i_size, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(i_size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* i_pointer)
{
	ASSERT(i_pointer);

	// calculate the address of the descriptor from the user's pointer
#ifdef BUILD_DEBUG
	uint8_t* descriptor_address = static_cast<uint8_t*>(i_pointer) - DEFAULT_GUARDBAND_SIZE - engine::memory::BlockAllocator::GetSizeOfBD();
#else
	uint8_t* descriptor_address = static_cast<uint8_t*>(i_pointer) - engine::memory::BlockAllocator::GetSizeOfBD();
#endif
	// cast to get a descriptor
	engine::memory::BD* descriptor = reinterpret_cast<engine::memory::BD*>(descriptor_address);
	ASSERT(descriptor);
	
	// get the allocator from the descriptor
	engine::memory::BlockAllocator* allocator = descriptor->allocator;
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p) on allocator-%d", i_pointer, allocator->GetID());
#endif

	// call free on the pointer
	allocator->Free(i_pointer);
}

void* operator new[](size_t i_size)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu) on allocator-%d", i_size, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(i_size);
	ASSERT(pointer);
	return pointer;
}

void operator delete[](void* i_pointer)
{
	ASSERT(i_pointer);

	// calculate the address of the descriptor from the user's pointer
#ifdef BUILD_DEBUG
	uint8_t* descriptor_address = static_cast<uint8_t*>(i_pointer) - DEFAULT_GUARDBAND_SIZE - engine::memory::BlockAllocator::GetSizeOfBD();
#else
	uint8_t* descriptor_address = static_cast<uint8_t*>(i_pointer) - engine::memory::BlockAllocator::GetSizeOfBD();
#endif
	// cast to get a descriptor
	engine::memory::BD* descriptor = reinterpret_cast<engine::memory::BD*>(descriptor_address);
	ASSERT(descriptor);

	// get the allocator from the descriptor
	engine::memory::BlockAllocator* allocator = descriptor->allocator;
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p) on allocator-%d", i_pointer, allocator->GetID());
#endif

	// call free on the pointer
	allocator->Free(i_pointer);
}

void* operator new(size_t i_size, engine::memory::AlignmentType i_alignment)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, alignemnt = %d) on allocator-%d", i_size, i_alignment, default_allocator->GetID());
#endif
	void* pointer = default_allocator->Alloc(i_size, i_alignment);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_pointer);
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, alignemnt = %d) on allocator-%d", i_pointer, i_alignment, default_allocator->GetID());
#endif
	default_allocator->Free(i_pointer);
}

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p) on allocator-%d", i_size, i_allocator, i_allocator->GetID());
#endif
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p) on allocator-%d", i_pointer, i_allocator, i_allocator->GetID());
#endif
	i_allocator->Free(i_pointer);
}

void* operator new[](size_t i_size, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu, allocator = %p) on allocator-%d", i_size, i_allocator, i_allocator->GetID());
#endif
	void* pointer = i_allocator->Alloc(i_size);
	ASSERT(pointer);
	return pointer;
}

void operator delete[](void* i_pointer, engine::memory::BlockAllocator* i_allocator)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete[](pointer = %p, allocator = %p) on allocator-%d", i_pointer, i_allocator, i_allocator->GetID());
#endif
	i_allocator->Free(i_pointer);
}

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p, alignment = %d) on allocator-%d", i_size, i_allocator, i_alignment, i_allocator->GetID());
#endif
	void* pointer = i_allocator->Alloc(i_size, i_alignment);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment)
{
	ASSERT(i_pointer);
	ASSERT(i_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p, alignment = %d) on allocator-%d", i_pointer, i_allocator, i_alignment, i_allocator->GetID());
#endif
	i_allocator->Free(i_pointer);
}

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t i_size, const char* i_file_name, unsigned int i_line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling new(size = %zu, file_name = %s, line = %d) on allocator-%d", i_size, i_file_name, i_line, default_allocator->GetID());
	void* pointer = default_allocator->Alloc(i_size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* i_pointer, const char* i_file_name, unsigned int i_line)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::CreateDefaultAllocator();
	ASSERT(default_allocator);
	LOG("Calling delete(pointer = %p, file_name = %s, line = %d) on allocator-%d", i_pointer, i_file_name, i_line, default_allocator->GetID());
	default_allocator->Free(i_pointer);
}
#endif
