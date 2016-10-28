#include "Allocator\BlockAllocator.h"
#include "Logger\Logger.h"

#include <stdlib.h>			// for _aligned_malloc & _aligned_free
#include <string.h>			// for memset

namespace engine {

#ifdef BUILD_DEBUG
unsigned int BlockDescriptor::counter_ = 0;
#endif

void BlockDescriptor::Init()
{
	block_pointer_ = NULL;
	block_size_ = 0;
	next_ = NULL;
#ifdef BUILD_DEBUG
	id_ = BlockDescriptor::counter_++;
#endif
}

BlockAllocator* BlockAllocator::instance_ = NULL;

BlockAllocator::BlockAllocator() : block_(NULL),
	total_block_size_(DEFAULT_BLOCK_SIZE),
	num_block_descriptors_(DEFAULT_NUM_BLOCK_DESCRIPTORS),
	pool_head_(NULL),
	user_list_head_(NULL),
	free_list_head_(NULL)
{
	Init();
}

BlockAllocator::BlockAllocator(const size_t block_size, const unsigned int num_block_descriptors) : block_(NULL),
	total_block_size_(block_size),
	num_block_descriptors_(num_block_descriptors),
	pool_head_(NULL),
	user_list_head_(NULL),
	free_list_head_(NULL)
{
	Init();
}

BlockAllocator::~BlockAllocator()
{
	_aligned_free(block_);
	block_ = NULL;
}

BlockAllocator* BlockAllocator::Create(const size_t block_size, const unsigned int num_block_descriptors)
{
	if (BlockAllocator::instance_ == NULL)
	{
		BlockAllocator::instance_ = new BlockAllocator(block_size, num_block_descriptors);
	}
	return BlockAllocator::instance_;
}

void BlockAllocator::Destroy()
{
	if (BlockAllocator::instance_ != NULL)
	{
		delete BlockAllocator::instance_;
		BlockAllocator::instance_ = NULL;
	}
}

void BlockAllocator::Init()
{
	// round the total block size to the nearest multiple of 4
	total_block_size_ += DEFAULT_BYTE_ALIGNMENT - (total_block_size_ % DEFAULT_BYTE_ALIGNMENT);

	block_ = static_cast<uint8_t*>(_aligned_malloc(total_block_size_, 4));
#ifdef BUILD_DEBUG
	memset(block_, 0, total_block_size_);
	LOG("Base start address:%p\tend address:%p", block_, (block_ + total_block_size_));
#endif

	InitBlockDescriptors();
	InitFirstBlockDescriptor();
}

void BlockAllocator::InitBlockDescriptors()
{
	// check if we have enough to allocate memory for the required number of block descriptors
	uint8_t* bd_begin = ((block_ + total_block_size_) - (num_block_descriptors_ * sizeof(BD)));
	ASSERT(Contains(bd_begin));

	// calculate the amount of memory available after separating memory for the block descriptors
	usable_block_size_ = total_block_size_ - (num_block_descriptors_ * sizeof(BD));

	// initialize the "unused" block descriptor pool head
	pool_head_ = reinterpret_cast<BD*>(bd_begin);
#ifdef BUILD_DEBUG
	LOG("Descriptor pool start address:%p\tend address:%p", pool_head_, (pool_head_ + num_block_descriptors_));
#endif

	// initialize the pool of block descriptors
	for (unsigned int i = 0; i < num_block_descriptors_; ++i)
	{
		// initialize the block descriptor
		BD* bd = (pool_head_ + i);
		bd->Init();

		// add it to the pool
		if (i > 0)
		{
			(bd - 1)->next_ = bd;
		}
	}
}

void BlockAllocator::InitFirstBlockDescriptor()
{
	// fetch a descriptor from the pool
	BD* first_bd = GetDescriptorFromPool();
	ASSERT(first_bd != NULL);

	// initialize the first descriptor
	first_bd->block_pointer_ = block_;
	first_bd->block_size_ = usable_block_size_;
	// add the descriptor to the free list
	AddToList(&free_list_head_, &first_bd);
}

BD* BlockAllocator::GetDescriptorFromPool()
{
	// return NULL if the pool is empty
	if (pool_head_ == NULL)
	{
		return NULL;
	}

	// save the head as the return
	BD* bd_ret = pool_head_;

	// advance the head
	pool_head_ = pool_head_->next_;

	// remove the element from the pool
	bd_ret->next_ = NULL;

	return bd_ret;
}

void BlockAllocator::AddDescriptorToPool(BD* bd)
{
	ASSERT(bd != NULL);

	// add the descriptor to the head
	bd->next_ = pool_head_;
	pool_head_ = bd;
}

BD* BlockAllocator::GetDescriptorFromFreeList(const size_t size)
{
	// loop the free list for a descriptor to a block that is big enough
	BD* curr_bd = free_list_head_;
	BD* prev_bd = NULL;
	while (curr_bd != NULL)
	{
		if (size <= curr_bd->block_size_)
		{
			// check if this is the first descriptor in the list
			if (prev_bd != NULL)
			{
				// not the first descriptor
				prev_bd->next_ = curr_bd->next_;
			}
			else
			{
				// update the head
				free_list_head_ = curr_bd->next_;
			}
			curr_bd->next_ = NULL;
			return curr_bd;
		}

		prev_bd = curr_bd;
		curr_bd = curr_bd->next_;
	}
	return NULL;
}

void BlockAllocator::AddToList(BD** head, BD** bd)
{
	ASSERT(head != NULL);
	ASSERT(bd != NULL);

	// check if the list is empty
	if (*head == NULL)
	{
		// add the new descriptor to the front
		(*bd)->next_ = *head;
		*head = *bd;
		return;
	}

	// if the new descriptor's block is smaller than the head's block, add it to the front
	if ((*bd)->block_pointer_ < (*head)->block_pointer_)
	{
		(*bd)->next_ = *head;
		*head = *bd;
	}
	else
	{
		// add this descriptor based on ascending order of block size
		BD* curr = *head;
		BD* prev = NULL;
		while (curr != NULL)
		{
			if ((*bd)->block_pointer_ < curr->block_pointer_)
			{
				if (prev != NULL)
				{
					prev->next_ = *bd;
				}
				(*bd)->next_ = curr;
				return;
			}
			prev = curr;
			curr = curr->next_;
		}

		// this means the descriptor's block is larger than all elements in the list
		prev->next_ = *bd;
	}
}

void BlockAllocator::RemoveFromList(BD** head, BD** prev, BD** curr)
{
	ASSERT(head != NULL);
	ASSERT(prev != NULL);
	ASSERT(curr != NULL);

	if (*prev == NULL)
	{
		// this means we're removing the head
		*head = (*curr)->next_;
	}
	else
	{
		(*prev)->next_ = (*curr)->next_;
	}

	(*curr)->next_ = NULL;
}

#ifdef BUILD_DEBUG
bool BlockAllocator::CheckMemoryOverwrite(BD* bd) const
{
	ASSERT(bd != NULL);

	unsigned int lower_byte_counter = 0, upper_byte_counter = 0;
	size_t search_size = DEFAULT_GUARDBAND_SIZE + DEFAULT_BYTE_ALIGNMENT + MAX_EXTRA_MEMORY;
	search_size = search_size > bd->block_size_ ? bd->block_size_ : search_size;
	for (unsigned int i = 0; i < search_size; ++i)
	{
		// check lower guardband
		lower_byte_counter += (*(bd->block_pointer_ + i) == GUARDBAND_FILL) ? 1 : 0;

		// check upper guardband
		upper_byte_counter += (*(bd->block_pointer_ + bd->block_size_ - i - 1) == GUARDBAND_FILL) ? 1 : 0;
	}

	bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
	if (found_overwrite)
	{
		LOG("Found overwrite!");
	}

	return found_overwrite;
}

void BlockAllocator::ClearBlock(BD* bd)
{
	ASSERT(bd != NULL);
	memset(bd->block_pointer_, 0, bd->block_size_);
}
#endif

void* BlockAllocator::Alloc(const size_t size)
{
	if (size <= 0)
	{
		LOG_ERROR("Bad input passed into Alloc!");
		return NULL;
	}

	// add padding for guardbanding AND byte alignment
#ifdef BUILD_DEBUG
	size_t size_with_padding = size + DEFAULT_GUARDBAND_SIZE * 2 + DEFAULT_BYTE_ALIGNMENT;
#else
	size_t size_with_padding = size + DEFAULT_BYTE_ALIGNMENT;
#endif

	// declare a block descriptor to service this request
	BD* new_bd = NULL;

	// 1. look for a descriptor in the free list
	BD* free_bd = GetDescriptorFromFreeList(size_with_padding);
	if (free_bd == NULL)
	{
		// we don't have a descriptor with enough memory so let's try and defrag
		Defragment();

		// request a new descriptor again after defragmentation
		free_bd = GetDescriptorFromFreeList(size_with_padding);
		if (free_bd == NULL)
		{
			LOG_ERROR("Insufficient memory!");
			return NULL;
		}
	}

	// 2. check if we need to split the memory pointed by the descriptor returned from 1.
	if (free_bd->block_size_ <= size_with_padding + MAX_EXTRA_MEMORY)
	{
		// 3. if we don't need to split, use the descriptor returned from 1 to service this request
		new_bd = free_bd;
	}
	else
	{
		// 4. if we need to split, get a new descriptor from the pool
		new_bd = GetDescriptorFromPool();
		if (new_bd == NULL)
		{
			// we used up all the descriptors so let's try and defrag
			Defragment();

			// request a new descriptor again after defragmentation
			new_bd = GetDescriptorFromPool();
			if (new_bd == NULL)
			{
				// can't proceed without a new descriptor
				// so add the descriptor from 1 back to the free list and return NULL
				AddToList(&free_list_head_, &free_bd);
				LOG_ERROR("Insufficient descriptors!");
				return NULL;				
			}
		}

		// calculate the address of the new block
		new_bd->block_pointer_ = free_bd->block_pointer_ + free_bd->block_size_ - size_with_padding;

		// adjust for byte alignment
		const uintptr_t adjustment = (reinterpret_cast<uintptr_t>(new_bd->block_pointer_)) % DEFAULT_BYTE_ALIGNMENT;
		new_bd->block_pointer_ += DEFAULT_BYTE_ALIGNMENT - adjustment;

		// reduce size by the adjustment
		new_bd->block_size_ = size_with_padding - (DEFAULT_BYTE_ALIGNMENT - adjustment);

		// splice the free block
		free_bd->block_size_ -= new_bd->block_size_;

		// add the remaining free block back to the free list
		AddToList(&free_list_head_, &free_bd);
	}

#ifdef BUILD_DEBUG
	// add guardbands
	for (unsigned int i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		*(new_bd->block_pointer_ + i) = GUARDBAND_FILL;
		*(new_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + size + i) = GUARDBAND_FILL;
	}
#endif

	// add the descriptor to the user list
	AddToList(&user_list_head_, &new_bd);

#ifdef BUILD_DEBUG
	return (new_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE);
#else
	return new_bd->block_pointer_;
#endif
}

// Deallocate a block of memory
bool BlockAllocator::Free(void* pointer)
{
	bool free_successful = false;

	// validate input
	if (pointer == NULL)
	{
		LOG_ERROR("Bad input provided to free!");
		return free_successful;
	}

	// search for the descriptor
	BD* curr_bd = user_list_head_;
	BD* prev_bd = NULL;
	while (curr_bd != NULL)
	{
		// calculate the pointer returned to the user
#ifdef BUILD_DEBUG
		uint8_t* user_pointer = curr_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE;
#else
		uint8_t* user_pointer = curr_bd->block_pointer_;
#endif

		if (user_pointer == pointer)
		{
#ifdef BUILD_DEBUG
			// check for overwrites
			ASSERT(!CheckMemoryOverwrite(curr_bd));
#endif

			// remove descriptor from user list			
			RemoveFromList(&user_list_head_, &prev_bd, &curr_bd);

#ifdef BUILD_DEBUG
			// clear the block
			ClearBlock(curr_bd);
#endif

			// add descriptor to free list
			AddToList(&free_list_head_, &curr_bd);

			free_successful = true;
			break;
		}

		prev_bd = curr_bd;
		curr_bd = curr_bd->next_;
	}

	return free_successful;
}

// Run defragmentation
void BlockAllocator::Defragment()
{
#ifdef BUILD_DEBUG
	LOG("Defragmenting...");
	unsigned int num_blocks_combined = 0;
	size_t bytes_combined = 0;
#endif

	BD* curr = free_list_head_;
	while (curr != NULL && curr->next_ != NULL)
	{
		if (curr->block_pointer_ + curr->block_size_ == curr->next_->block_pointer_)
		{
			BD* next_bd = curr->next_;

#ifdef BUILD_DEBUG
			LOG("Merging %zu bytes from block-%d & block-%d", (curr->block_size_ + next_bd->block_size_), curr->id_, next_bd->id_);
#endif
			// update current descriptor's block size
			curr->block_size_ += next_bd->block_size_;

#ifdef BUILD_DEBUG
			bytes_combined += next_bd->block_size_;
#endif
			// update current descriptor's next pointer
			curr->next_ = next_bd->next_;

			// clear next descriptor's block size
			next_bd->block_size_ = 0;
			// clear next descriptor's next pointer
			next_bd->next_ = NULL;

			AddDescriptorToPool(next_bd);

#ifdef BUILD_DEBUG
			++num_blocks_combined;
#endif
			continue;
		}
		curr = curr->next_;
	}

#ifdef BUILD_DEBUG
	if (bytes_combined > 0)
	{
		LOG("Defragment finished...merged %zu bytes from %d blocks!", bytes_combined, num_blocks_combined);
	}
	else
	{
		LOG("Defragment finished...could not find any contiguous blocks!");
	}
#endif
}

// Query whether a given pointer is within this allocator's range
bool BlockAllocator::Contains(const void* pointer) const
{
	return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) < (block_ + usable_block_size_));
}

// Query whether a given pointer is a user allocation
bool BlockAllocator::IsAllocated(const void* pointer) const
{
	BD* bd = user_list_head_;
	while (bd != NULL)
	{
		// check if the pointer passed in exists within each descriptor
		if ((static_cast<const uint8_t*>(pointer) >= bd->block_pointer_ && static_cast<const uint8_t*>(pointer) < (bd->block_pointer_ + bd->block_size_)))
		{
			return true;
		}
		bd = bd->next_;
	}
	return false;
}

const size_t BlockAllocator::GetLargestFreeBlockSize() const
{
	size_t largest_size = 0;
	// loop the free list
	BD* bd = free_list_head_;
	while (bd != NULL)
	{
		// check actual block size not user block size
		if (bd->block_size_ > largest_size)
		{
			largest_size = bd->block_size_;
		}
		bd = bd->next_;
	}

	// add padding for guardbanding AND byte alignment
#ifdef BUILD_DEBUG
	largest_size -= DEFAULT_GUARDBAND_SIZE * 2;
#endif
	largest_size -= DEFAULT_BYTE_ALIGNMENT;

	return largest_size;
}

const size_t BlockAllocator::GetTotalFreeMemorySize() const
{
	size_t total_size = 0;
	// loop the free list
	BD* bd = free_list_head_;
	while (bd != NULL)
	{
		total_size += bd->block_size_;
		bd = bd->next_;
	}

	return total_size;
}

#ifdef BUILD_DEBUG
void BlockAllocator::PrintAllDescriptors() const
{
	LOG("---------- %s ----------", __FUNCTION__);
	if (pool_head_ != NULL)
	{
		unsigned int count = 0;
		LOG("POOL:");
		for (BD* bd = pool_head_; bd != NULL; bd = bd->next_)
		{
			LOG("BD.id:%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG("POOL SIZE:%d", count);
	}

	PrintFreeDescriptors();
	PrintUsedDescriptors();	
	LOG("---------- END ----------");
}

void BlockAllocator::PrintFreeDescriptors() const
{
	if (free_list_head_ != NULL)
	{
		LOG("---------- %s ----------", __FUNCTION__);
		unsigned int count = 0;
		LOG("FREE:");
		for (BD* bd = free_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG("BD.id:%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG("FREE LIST SIZE:%d", count);
		LOG("---------- END ----------");
	}
}

void BlockAllocator::PrintUsedDescriptors() const
{
	if (user_list_head_ != NULL)
	{
		LOG("---------- %s ----------", __FUNCTION__);
		unsigned int count = 0;
		LOG("USER:");
		for (BD* bd = user_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG("BD.id:%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG("USER LIST SIZE:%d", count);
		LOG("---------- END ----------");
	}
}
#endif

} // namespace engine