#ifndef ENGINE_BLOCK_ALLOCATOR_H_
#define ENGINE_BLOCK_ALLOCATOR_H_

#include <stdint.h>

namespace engine {

#define DEFAULT_BLOCK_SIZE 1024 * 1024
#define DEFAULT_NUM_BLOCK_DESCRIPTORS 512
#define DEFAULT_GUARDBAND_SIZE 4
#define GUARDBAND_FILL 0xFD
#define DEFAULT_BYTE_ALIGNMENT 4
#define MAX_EXTRA_MEMORY 8

#ifdef BUILD_DEBUG
enum class DescriptorListType {
	DESCRIPTOR_TYPE_POOL = 1 << 0,
	DESCRIPTOR_TYPE_FREE = 1 << 1,
	DESCRIPTOR_TYPE_USER = 1 << 2
};
#define DESCRIPTOR_TYPE_ALL (DescriptorListType)(1 << 0 | 1 << 1 | 1 << 2)
#endif

/*
	BlockDescriptor
	A block descriptor describes a block of memory that is managed by the block allocator.
	- It contains a pointer to a block of memory as well as its size
	- It contains a pointer to the next descriptor in a list or NULL if its not part of a list
*/
typedef struct BlockDescriptor
{
public:
	BlockDescriptor* next_;				// pointer to the next block descriptor
	uint8_t* block_pointer_;			// pointer to the actual block of data
	size_t block_size_;					// size of the actual block of data

#ifdef BUILD_DEBUG
	static unsigned int counter_;		// a counter to keep track of all the descriptors
	unsigned int id_;					// an identifier for each descriptor
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
	BlockAllocator(const size_t block_size = DEFAULT_BLOCK_SIZE, const unsigned int num_block_descriptors = DEFAULT_NUM_BLOCK_DESCRIPTORS);
	~BlockAllocator();
	static BlockAllocator* instance_;

	void Init();
	void InitBlockDescriptors();
	void InitFirstBlockDescriptor();

	BD* GetDescriptorFromPool();
	void AddDescriptorToPool(BD* bd);
	BD* GetDescriptorFromFreeList(const size_t size);

	void AddToList(BD** head, BD** bd);
	void RemoveFromList(BD** head, BD** prev, BD** curr);

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(BD* bd) const;
	void ClearBlock(BD* bd);
#endif

public:
	static BlockAllocator* Create(const size_t block_size = DEFAULT_BLOCK_SIZE, const unsigned int num_block_descriptors = DEFAULT_NUM_BLOCK_DESCRIPTORS);
	static void Destroy();

	// Allocate a block of memory with given size
	void* Alloc(const size_t size);
	// Deallocate a block of memory
	bool Free(void* pointer);

	// Run defragmentation
	void Defragment();

	// Query whether a given pointer is within this allocator's range
	bool Contains(const void* pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* pointer) const;

	const size_t GetLargestFreeBlockSize() const;
	const size_t GetTotalFreeMemorySize() const;

#ifdef BUILD_DEBUG
	void PrintAllDescriptors() const;
	void PrintFreeDescriptors() const;
	void PrintUsedDescriptors() const;
#endif

protected:	
	BD* pool_head_;										// pool of unused block descriptors
	BD* free_list_head_;								// list of block descriptors describing free blocks
	BD* user_list_head_;								// list of block descriptors describing allocated blocks
	
	uint8_t* block_;									// actual block of memory
	size_t total_block_size_;							// total size of block (including memory for block descriptors)
	size_t usable_block_size_;							// size of memory to use for user allocations (excluding memory for block descriptors)
	unsigned int num_block_descriptors_;				// initial number of block descriptors

}; // class BlockAllocator

} // namespace engine

#endif // ENGINE_BLOCK_ALLOCATOR_H_