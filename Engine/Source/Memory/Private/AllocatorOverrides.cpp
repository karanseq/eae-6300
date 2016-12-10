#include "Memory\AllocatorOverrides.h"

#include "Memory\BlockAllocator.h"
#include "Memory\FixedSizeAllocator.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"

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
	void* pointer = nullptr;

	// loop over the registered fixed size allocators to find the best fit
	// this operator assumes that the FSAs are registered in ascending order of the size of the blocks they maintain
	engine::memory::FixedSizeAllocator** const registered_fsas = engine::memory::FixedSizeAllocator::GetRegisteredFixedSizeAllocators();
	for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
	{
		// if the FSA exists and is big enough to service this request
		if (registered_fsas[i] && registered_fsas[i]->GetBlockSize() >= size)
		{
			pointer = registered_fsas[i]->Alloc(size);
			if (pointer)
			{
#ifdef BUILD_DEBUG
				LOG("Calling new(size = %zu) on FixedSizeAllocator-%d with fixed_block_size:%zu", size, registered_fsas[i]->GetID(), registered_fsas[i]->GetBlockSize());
#endif
				return pointer;
			}
			// at this point, we're choosing to try and allocate using the next available FSA.
			// we could choose to break out of this loop and allocate using the default block allocator instead.
		}
	}

	// service this request using the default block allocator
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);

#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu) on BlockAllocator-%d", size, default_allocator->GetID());
#endif
	
	pointer = default_allocator->Alloc(size);
	ASSERT(pointer);

	return pointer;
}

void operator delete(void* pointer)
{
	ASSERT(pointer);

	// get all registered fixed size allocators
	engine::memory::FixedSizeAllocator** const fixed_size_allocators = engine::memory::FixedSizeAllocator::GetRegisteredFixedSizeAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_fixed_size_allocators = MAX_FIXED_SIZE_ALLOCATORS;
	while (num_fixed_size_allocators > 0)
	{
		if (fixed_size_allocators[num_fixed_size_allocators - 1] && fixed_size_allocators[num_fixed_size_allocators - 1]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", pointer, fixed_size_allocators[num_fixed_size_allocators - 1]->GetID(), fixed_size_allocators[num_fixed_size_allocators - 1]->GetBlockSize());
#endif
			return;
		}
		--num_fixed_size_allocators;
	}

	// get all registered block allocators
	engine::memory::BlockAllocator** const block_allocators = engine::memory::BlockAllocator::GetRegisteredBlockAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_block_allocators = MAX_BLOCK_ALLOCATORS - 1;
	while (num_block_allocators >= 0)
	{
		if (block_allocators[num_block_allocators] && block_allocators[num_block_allocators]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on BlockAllocator-%d", pointer, block_allocators[num_block_allocators]->GetID());
#endif
			return;
		}
		--num_block_allocators;
	}

	// this means the pointer could not be deleted
	LOG_ERROR("Could not delete(pointer = %p) on any of the allocators!", pointer);
}

void* operator new[](size_t size)
{
	void* pointer = nullptr;

	// loop over the registered fixed size allocators to find the best fit
	// this operator assumes that the FSAs are registered in ascending order of the size of the blocks they maintain
	engine::memory::FixedSizeAllocator** const registered_fsas = engine::memory::FixedSizeAllocator::GetRegisteredFixedSizeAllocators();
	for (uint8_t i = 0; i < MAX_FIXED_SIZE_ALLOCATORS; ++i)
	{
		// if the FSA exists and is big enough to service this request
		if (registered_fsas[i] && registered_fsas[i]->GetBlockSize() >= size)
		{
			pointer = registered_fsas[i]->Alloc(size);
			if (pointer)
			{
#ifdef BUILD_DEBUG
				LOG("Calling new(size = %zu) on FixedSizeAllocator-%d with fixed_block_size:%zu", size, registered_fsas[i]->GetID(), registered_fsas[i]->GetBlockSize());
#endif
				return pointer;
			}
			// at this point, we're choosing to try and allocate using the next available FSA.
			// we could choose to break out of this loop and allocate using the default block allocator instead.
		}
	}

	// service this request using the default block allocator
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);

#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu) on BlockAllocator-%d", size, default_allocator->GetID());
#endif

	pointer = default_allocator->Alloc(size);
	ASSERT(pointer);

	return pointer;
}

void operator delete[](void* pointer)
{
	ASSERT(pointer);

	// get all registered fixed size allocators
	engine::memory::FixedSizeAllocator** const fixed_size_allocators = engine::memory::FixedSizeAllocator::GetRegisteredFixedSizeAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_fixed_size_allocators = MAX_FIXED_SIZE_ALLOCATORS;
	while (num_fixed_size_allocators >= 0)
	{
		if (fixed_size_allocators[num_fixed_size_allocators - 1] && fixed_size_allocators[num_fixed_size_allocators - 1]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", pointer, fixed_size_allocators[num_fixed_size_allocators - 1]->GetID(), fixed_size_allocators[num_fixed_size_allocators - 1]->GetBlockSize());
#endif
			return;
		}
		--num_fixed_size_allocators;
	}

	// get all registered block allocators
	engine::memory::BlockAllocator** const block_allocators = engine::memory::BlockAllocator::GetRegisteredBlockAllocators();

	// free the pointer from the appropriate allocator
	uint8_t num_block_allocators = MAX_BLOCK_ALLOCATORS - 1;
	while (num_block_allocators >= 0)
	{
		if (block_allocators[num_block_allocators] && block_allocators[num_block_allocators]->Free(pointer))
		{
#ifdef BUILD_DEBUG
			LOG("Called delete(pointer = %p) on BlockAllocator-%d", pointer, block_allocators[num_block_allocators]->GetID());
#endif
			return;
		}
		--num_block_allocators;
	}

	// this means the pointer could not be deleted
	LOG_ERROR("Could not delete(pointer = %p) on any of the allocators!", pointer);
}

void* operator new(size_t size, engine::memory::AlignmentType alignment)
{
	engine::memory::BlockAllocator* default_allocator = engine::memory::BlockAllocator::GetDefaultAllocator();
	ASSERT(default_allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, alignemnt = %d) on the default allocator", size, alignment);
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
	LOG("Calling delete(pointer = %p, alignemnt = %d) on the default allocator", pointer, alignment);
#endif
	default_allocator->Free(pointer);
}

void* operator new(size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p) on BlockAllocator-%d", size, allocator, allocator->GetID());
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
	LOG("Calling delete(pointer = %p, allocator = %p) on BlockAllocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new[](size_t size, engine::memory::BlockAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu, allocator = %p) on BlockAllocator-%d", size, allocator, allocator->GetID());
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
	LOG("Calling delete[](pointer = %p, allocator = %p) on BlockAllocator-%d", pointer, allocator, allocator->GetID());
#endif
	allocator->Free(pointer);
}

void* operator new(size_t size, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new(size = %zu, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", size, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete(void* pointer, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete(pointer = %p, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", pointer, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
	allocator->Free(pointer);
}

void* operator new[](size_t size, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling new[](size = %zu, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", size, allocator, allocator->GetID(), allocator->GetBlockSize());
#endif
	void* pointer = allocator->Alloc(size);
	ASSERT(pointer);
	return pointer;
}

void operator delete[](void* pointer, engine::memory::FixedSizeAllocator* allocator)
{
	ASSERT(pointer);
	ASSERT(allocator);
#ifdef BUILD_DEBUG
	LOG("Calling delete[](pointer = %p, allocator = %p) on FixedSizeAllocator-%d with fixed_block_size:%zu", pointer, allocator, allocator->GetID(), allocator->GetBlockSize());
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
