#ifndef CUSTOM_NEW_H_
#define CUSTOM_NEW_H_

#include "Memory\BlockAllocator.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace memory {

	enum AlignmentType
	{
		ALIGNMENT_DEFAULT = 4,
		ALIGNMENT_8 = 8,
		ALIGNMENT_16 = 16,
		ALIGNMENT_32 = 32,
		ALIGNMENT_64 = 64
	};

} // namespace memory
} // namespace engine

//void* malloc(size_t i_size);
//void free(void* i_pointer);

void* operator new(size_t i_size);
void operator delete(void* i_pointer);

void* operator new[](size_t i_size);
void operator delete[](void* i_pointer);

void* operator new(size_t i_size, engine::memory::AlignmentType i_alignment);
void operator delete(void* i_pointer, engine::memory::AlignmentType i_alignment);

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator);
void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator);

void* operator new[](size_t i_size, engine::memory::BlockAllocator* i_allocator);
void operator delete[](void* i_pointer, engine::memory::BlockAllocator* i_allocator);

void* operator new(size_t i_size, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment);
void operator delete(void* i_pointer, engine::memory::BlockAllocator* i_allocator, engine::memory::AlignmentType i_alignment);

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t i_size, const char* i_file_name, unsigned int i_line);
void operator delete(void* i_pointer, const char* i_file_name, unsigned int i_line);
#else
#define TRACK_NEW
#endif

#endif // CUSTOM_NEW_H_