#ifndef CUSTOM_NEW_H_
#define CUSTOM_NEW_H_

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

	// forward declaration
	class BlockAllocator;
	class FixedSizeAllocator;

} // namespace memory
} // namespace engine

//void* malloc(size_t size);
//void free(void* pointer);

void* operator new(size_t size);
void operator delete(void* pointer);

void* operator new[](size_t size);
void operator delete[](void* pointer);

void* operator new(size_t size, engine::memory::AlignmentType alignment);
void operator delete(void* pointer, engine::memory::AlignmentType alignment);

void* operator new(size_t size, engine::memory::BlockAllocator* allocator);
void operator delete(void* pointer, engine::memory::BlockAllocator* allocator);

void* operator new[](size_t size, engine::memory::BlockAllocator* allocator);
void operator delete[](void* pointer, engine::memory::BlockAllocator* allocator);

void* operator new(size_t size, engine::memory::FixedSizeAllocator* allocator);
void operator delete(void* pointer, engine::memory::FixedSizeAllocator* allocator);

void* operator new[](size_t size, engine::memory::FixedSizeAllocator* allocator);
void operator delete[](void* pointer, engine::memory::FixedSizeAllocator* allocator);

void* operator new(size_t size, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment);
void operator delete(void* pointer, engine::memory::BlockAllocator* allocator, engine::memory::AlignmentType alignment);

#ifdef BUILD_DEBUG
#define TRACK_NEW (__FILE__, __LINE__)
void* operator new(size_t size, const char* file_name, unsigned int line);
void operator delete(void* pointer, const char* file_name, unsigned int line);
#else
#define TRACK_NEW
#endif

#endif // CUSTOM_NEW_H_