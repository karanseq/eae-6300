#ifndef ENGINE_BLOCK_ALLOCATOR_H_
#define ENGINE_BLOCK_ALLOCATOR_H_

#include <stdint.h>

namespace engine {

#define DEFAULT_BLOCK_SIZE 1024 * 1024
#define DEFAULT_GUARDBAND_SIZE 4
#define DEFAULT_BYTE_ALIGNMENT 4
#define MAX_EXTRA_MEMORY 8

#define GUARDBAND_FILL		0xFD
#define DEAD_FILL			0xDD
#define CLEAN_FILL			0xCD

/*
	BlockDescriptor
	A block descriptor describes a block of memory that is managed by the block allocator.
	- It contains a pointer to a block of memory as well as its size
	- It contains a pointer to the next & previous descriptor in a list or NULL if its not part of a list
*/
typedef struct BlockDescriptor
{
public:
	BlockDescriptor* next_;				// pointer to the next block descriptor
	BlockDescriptor* previous_;			// pointer to the previous block descriptor
	uint8_t* block_pointer_;			// pointer to the actual block of data
	size_t block_size_;					// size of the actual block of data

#ifdef BUILD_DEBUG
	size_t user_size_;					// size of the block requested by the user
	unsigned int id_;					// an identifier for each descriptor
	static unsigned int counter_;		// a counter to keep track of all the descriptors
#endif

	void Init();
} BD;

/*
	BlockAllocator
	A simple block allocator that users can use to request for memory.
	- Users need to provide the size of the memory block they need.
	- If the request was successful, users will receive a pointer to the start of the memory block.
	- If the request was unsuccessful, users will receive a NULL pointer.
*/
class BlockAllocator
{
protected:
	BlockAllocator();
	BlockAllocator(const size_t block_size = DEFAULT_BLOCK_SIZE);
	~BlockAllocator();
	static BlockAllocator* instance_;

	void Init();
	void InitFirstBlockDescriptor();

	void AddToList(BD** head, BD** bd, bool enable_sort);
	void RemoveFromList(BD** head, BD** bd);

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(BD* bd) const;
	inline void ClearBlock(BD* bd, const unsigned char fill);
#endif

public:
	static BlockAllocator* Create(const size_t block_size = DEFAULT_BLOCK_SIZE);
	static void Destroy();

	// Allocate a block of memory with given size
	void* Alloc(const size_t size, const size_t alignment = DEFAULT_BYTE_ALIGNMENT);
	// Deallocate a block of memory
	bool Free(void* pointer);

	// Run defragmentation
	void Defragment();

	// Query whether a given pointer is within this allocator's range
	inline bool Contains(const void* pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* pointer) const;

	const size_t GetLargestFreeBlockSize(const size_t alignment = DEFAULT_BYTE_ALIGNMENT) const;
	const size_t GetTotalFreeMemorySize() const;

#ifdef BUILD_DEBUG
	void PrintAllDescriptors() const;
	void PrintFreeDescriptors() const;
	void PrintUsedDescriptors() const;
#endif

protected:
	uint8_t* block_;									// actual block of memory
	
	BD* free_list_head_;								// list of block descriptors describing free blocks
	BD* user_list_head_;								// list of block descriptors describing allocated blocks
	
	size_t total_block_size_;							// total size of block
	size_t size_of_BD_;									// size of a BlockDescriptor object

}; // class BlockAllocator

} // namespace engine

#include "BlockAllocator-inl.h"

#endif // ENGINE_BLOCK_ALLOCATOR_H_