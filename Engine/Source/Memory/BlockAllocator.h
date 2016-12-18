#ifndef ENGINE_BLOCK_ALLOCATOR_H_
#define ENGINE_BLOCK_ALLOCATOR_H_

#include <stdint.h>

namespace engine {
namespace memory {

#define DEFAULT_BLOCK_SIZE				1024 * 1024
#define DEFAULT_GUARDBAND_SIZE			4
#define DEFAULT_BYTE_ALIGNMENT			4
#define MAX_EXTRA_MEMORY				8

#define GUARDBAND_FILL					0xFD
#define DEAD_FILL						0xDD
#define CLEAN_FILL						0xCD

/*
	BlockDescriptor
	A block descriptor describes a block of memory that is managed by the block allocator.
	- It contains a pointer to a block of memory as well as its size
	- It contains a pointer to the next & previous descriptor in a list or nullptr if its not part of a list
*/
class BlockAllocator;
typedef struct BlockDescriptor
{
public:
	BlockAllocator*			allocator;				// pointer to the allocator managing this descriptor
	BlockDescriptor*		next;					// pointer to the next block descriptor
	BlockDescriptor*		previous;				// pointer to the previous block descriptor
	uint8_t*				block_pointer;			// pointer to the actual block of data
	size_t					block_size;				// size of the actual block of data

#ifdef BUILD_DEBUG
	size_t					user_size;				// size of the block requested by the user
	uint32_t				id;						// an identifier for each descriptor
	static uint32_t			counter;				// a counter to keep track of all the descriptors
#endif

	void Init();
} BD;

/*
	BlockAllocator
	A simple block allocator that users can use to request for memory.
	- Users need to provide the size of the memory block they need.
	- If the request was successful, users will receive a pointer to the start of the memory block.
	- If the request was unsuccessful, users will receive a nullptr pointer.
*/
class BlockAllocator
{
private:
	// disable default constructor, copy constructor & assignment operator
	BlockAllocator() {}
	BlockAllocator(const BlockAllocator& i_copy);
	BlockAllocator& operator=(const BlockAllocator& i_ba);

	BlockAllocator(void* i_memory, size_t i_block_size);
	~BlockAllocator() {}

	void InitFirstBlockDescriptor();

	void AddToList(BD** i_head, BD** i_bd, bool i_enable_sort);
	void RemoveFromList(BD** i_head, BD** i_bd);

#ifdef BUILD_DEBUG
	bool CheckMemoryOverwrite(BD* i_bd) const;
	inline void ClearBlock(BD* i_bd, const unsigned char i_fill);
#endif

public:
	static BlockAllocator* Create(void* i_memory, size_t i_block_size);
	static void Destroy(BlockAllocator* i_allocator);

	static BlockAllocator* CreateDefaultAllocator();
	static void DestroyDefaultAllocator();

	// Allocate a block of memory with given size
	void* Alloc(const size_t i_size, const size_t i_alignment = DEFAULT_BYTE_ALIGNMENT);
	// Deallocate a block of memory
	bool Free(void* i_pointer);

	// Run defragmentation
	void Defragment();

	// Query whether a given pointer is within this allocator's range
	inline bool Contains(const void* i_pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(const void* i_pointer) const;

	static inline size_t GetSizeOfBD();
	const size_t GetLargestFreeBlockSize(const size_t i_alignment = DEFAULT_BYTE_ALIGNMENT) const;
	const size_t GetTotalFreeMemorySize() const;

#ifdef BUILD_DEBUG
	inline unsigned int GetID() const;
	void PrintAllDescriptors() const;
	void PrintFreeDescriptors() const;
	void PrintUsedDescriptors() const;
#endif

private:
	uint8_t*										block_;									// actual block of memory
	
	BD*												free_list_head_;						// list of block descriptors describing free blocks
	BD*												user_list_head_;						// list of block descriptors describing allocated blocks
	
	size_t											total_block_size_;						// total size of block
	static											size_t size_of_BD_;						// size of a BlockDescriptor object

#ifdef BUILD_DEBUG
	uint32_t										id_;									// an id to keep track of this allocator in debug mode
	static uint32_t									counter_;								// a counter that will be used while setting ids for allocators
#endif

	static BlockAllocator*							default_allocator_;						// a reference to the default allocator

}; // class BlockAllocator

} // namespace memory
} // namespace engine

#include "BlockAllocator-inl.h"

#endif // ENGINE_BLOCK_ALLOCATOR_H_