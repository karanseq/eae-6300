#ifndef FIXED_SIZE_ALLOCATOR_H_
#define FIXED_SIZE_ALLOCATOR_H_

// library includes
#include <stdint.h>

// engine includes
#include "AllocatorUtil.h"

// forward declaration
namespace engine {
namespace data {
	class BitArray;
}
}

namespace engine {
namespace memory {

#define MAX_FIXED_SIZE_ALLOCATORS 15

// forward declarations
class BlockAllocator;

/*
	FixedSizeAllocator
	A simple fixed size allocator that users can use to request for memory of a fixed size.
	- Users need to provide the size of each fixed size block, the total number of blocks and a block allocator to allocate necessary memory from.
	- If the request was successful, users will receive a pointer to the start of the memory block.
	- If the request was unsuccessful, users will receive nullptr.
*/
class FixedSizeAllocator
{
private:
	// disable default constructor, copy constructor & assignment operator
	FixedSizeAllocator() {}
	FixedSizeAllocator(const FixedSizeAllocator& copy);
	FixedSizeAllocator& operator=(const FixedSizeAllocator& fsa);

	FixedSizeAllocator(void* memory, size_t total_block_size, size_t fixed_block_size, size_t num_blocks, BlockAllocator* allocator);
	~FixedSizeAllocator();

	inline uint8_t* GetPointerForBlock(size_t bit_index) const;

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(size_t bit_index) const;
	inline void ClearBlock(size_t bit_index, const unsigned char fill);
#endif

public:
	static FixedSizeAllocator* Create(size_t block_size, size_t num_blocks, BlockAllocator* allocator);
	static void Destroy(FixedSizeAllocator* allocator);

	static bool IsFixedSizeAllocatorRegistered(FixedSizeAllocator* allocator);
	static bool RegisterFixedSizeAllocator(FixedSizeAllocator* allocator);
	static bool DeregisterFixedSizeAllocator(FixedSizeAllocator* allocator);
	static inline FixedSizeAllocator** const GetRegisteredFixedSizeAllocators();

	// allocate a block of fixed size
	void* Alloc();
	// allocate a block of given size
	void* Alloc(const size_t size);
	// deallocate a block of memory
	bool Free(void* pointer);

	// Query whether a given pointer is within this allocator's range
	inline bool Contains(const void* pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* pointer) const;

	inline size_t GetBlockSize() const;
	inline size_t GetNumBlocks() const;

#ifdef BUILD_DEBUG
	inline unsigned int GetID() const;
#endif

private:
	uint8_t*										block_;													// pointer to the actual block of memory
	size_t											total_block_size_;										// total size of the entire block
	size_t											fixed_block_size_;										// size of each fixed block
	size_t											num_blocks_;											// total number of fixed blocks
	BlockAllocator*									block_allocator_;										// the block allocator used for the initial allocation
	engine::data::BitArray*							block_state_;											// a bit array to maintain the state (available = 0, allocated = 1) of each block of memory

#ifdef BUILD_DEBUG
	uint8_t											id_;													// an id to keep track of this allocator in debug mode
	static uint8_t									counter_;												// a counter that will be used while setting ids for allocators
#endif

	static FixedSizeAllocator*						registered_allocators_[MAX_FIXED_SIZE_ALLOCATORS];		// an array of pointers to all registered fixed size allocators

}; // class FixedSizeAllocator

} // namespace memory
} // namespace engine

#include "FixedSizeAllocator-inl.h"

#endif // FIXED_SIZE_ALLOCATOR_H_