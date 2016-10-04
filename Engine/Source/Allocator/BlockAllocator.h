#ifndef ENGINE_BLOCK_ALLOCATOR_H_
#define ENGINE_BLOCK_ALLOCATOR_H_

namespace engine {

#define DEFAULT_BLOCK_SIZE 1024 * 1024
#define DEFAULT_NUM_BLOCK_DESCRIPTORS 512
#define DEFAULT_GUARDBAND_SIZE 4
#define GUARDBAND_FILL 0xFD

/*
	BlockDescriptor
	A block descriptor describes a block of memory that is managed by the block allocator.
	- It contains pointers to the actual block of memory as well as a pointer that is eturned to the user.
	- It contains the size of the actual block of memory as well as the size of the block returned to the user.
*/
typedef struct BlockDescriptor
{
public:
	unsigned char* block_pointer_;		// pointer to the actual block of data
	unsigned char* user_pointer_;		// pointer to the block returned to user
	size_t block_size_;					// size of the actual block of data
	size_t user_size_;					// size of the block returned to the user
	BlockDescriptor* next_;				// pointer to the next block descriptor

#ifdef BUILD_DEBUG
	static unsigned int counter_;
	unsigned int id_;
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
	BlockAllocator(size_t block_size = DEFAULT_BLOCK_SIZE, unsigned int num_block_descriptors = DEFAULT_NUM_BLOCK_DESCRIPTORS);
	~BlockAllocator();
	static BlockAllocator* instance_;

	void Init();
	void InitBlockDescriptors();
	void InitFirstBlockDescriptor();

	BD* GetDescriptorFromPool();
	bool AddToPool(BD* bd);
	bool RemoveFromPool(BD* bd);
	bool AddToFreeList(BD* bd);
	bool RemoveFromFreeList(BD* bd);
	bool AddToOutstandingList(BD* bd);
	bool RemoveFromOutstandingList(BD* bd);

public:
	static BlockAllocator* Create(size_t block_size = DEFAULT_BLOCK_SIZE, unsigned int num_block_descriptors = DEFAULT_NUM_BLOCK_DESCRIPTORS);
	static void Destroy();

	// Allocate a block of memory with given size
	void* Alloc(size_t size);
	// Allocate a block of memory with given size and byte alignment
	void* Alloc(size_t size, unsigned int alignment);
	// Deallocate a block of memory
	bool Free(void* pointer);

	// Run garbage collection
	void Collect();

	// Query whether a given pointer is within this allocator's range
	bool Contains(void* pointer) const;
	// Query whether a given pointer is an outstanding allocation
	bool IsAllocated(void* pointer) const;

	size_t GetLargestFreeBlockSize() const;
	size_t GetTotalFreeMemorySize() const;

#ifdef BUILD_DEBUG
	void PrintAllDescriptors() const;
	void InitTestBlockDescriptor();
#endif

protected:
	unsigned char* block_;								// actual block of memory
	size_t total_block_size_;							// total size of block (including memory for block descriptors)
	size_t usable_block_size_;							// size of memory to use for user allocations (excluding memory for block descriptors)
	unsigned int num_block_descriptors_;				// initial number of block descriptors
	
	unsigned int pool_size_;							// current size of the pool of unused block descriptors
	BD* pool_head_;										// pool of unused block descriptors
	unsigned int free_list_size_;						// current size of the list of block descriptors describing free blocks
	BD* free_list_head_;								// list of block descriptors describing free blocks
	unsigned int outstanding_list_size_;				// current size of the list of block descriptors describing allocated blocks
	BD* outstanding_list_head_;							// list of block descriptors describing allocated blocks

}; // class BlockAllocator

} // namespace engine

#endif // ENGINE_BLOCK_ALLOCATOR_H_