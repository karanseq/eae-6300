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
	user_pointer_ = NULL;
	block_size_ = 0;
	user_size_ = 0;
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
	outstanding_list_head_(NULL),
	free_list_head_(NULL)
{
	Init();
}

BlockAllocator::BlockAllocator(const size_t block_size, const unsigned int num_block_descriptors) : block_(NULL),
	total_block_size_(block_size),
	num_block_descriptors_(num_block_descriptors),
	pool_head_(NULL),
	outstanding_list_head_(NULL),
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
	block_ = (unsigned char*)_aligned_malloc(total_block_size_, 4);
	memset(block_, 0, total_block_size_);
	LOG_DEBUG("Base start address:%p\tend address:%p", block_, (block_ + total_block_size_));

	InitBlockDescriptors();
	InitFirstBlockDescriptor();
}

void BlockAllocator::InitBlockDescriptors()
{
	// check if we have enough to allocate memory for the required number of block descriptors
	unsigned char* bd_begin = ((block_ + total_block_size_) - (num_block_descriptors_ * sizeof(BD)));
	ASSERT(Contains(bd_begin));

	// calculate the amount of memory available after separating memory for the block descriptors
	usable_block_size_ = total_block_size_ - (num_block_descriptors_ * sizeof(BD));

	// initialize the "unused" block descriptor pool head
	pool_head_ = (BD*)bd_begin;
	LOG_DEBUG("Descriptor pool start address:%p\tend address:%p", pool_head_, (pool_head_ + num_block_descriptors_));

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
	//AddToFreeList(first_bd);
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

BD* BlockAllocator::GetDescriptorFromFreeList(const size_t size)
{
	// loop the free list for a descriptor to a block that is big enough
	BD* curr_bd = free_list_head_;
	BD* prev_bd = NULL;
	while (curr_bd != NULL)
	{
		if (size < curr_bd->block_size_)
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
	if ((*bd)->block_size_ < (*head)->block_size_)
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
			if ((*bd)->block_size_ < curr->block_size_)
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

//bool BlockAllocator::AddToFreeList(BD* bd)
//{
//	ASSERT(bd != NULL);
//
//	// check if the list is empty
//	if (free_list_head_ == NULL)
//	{
//		// add the new descriptor to the front 
//		bd->next_ = free_list_head_;
//		free_list_head_ = bd;
//		return true;
//	}
//
//	// if the new descriptor's block is smaller than the head's block, add it to the front
//	if (bd->block_size_ < free_list_head_->block_size_)
//	{
//		bd->next_ = free_list_head_;
//		free_list_head_ = bd;
//	}
//	else
//	{
//		// add this descriptor based on ascending order of block size
//		BD* curr_bd = free_list_head_;
//		BD* prev_bd = NULL;
//		while (curr_bd != NULL)
//		{
//			if (bd->block_size_ < curr_bd->block_size_)
//			{
//				if (prev_bd != NULL)
//				{
//					prev_bd->next_ = bd;
//				}
//				bd->next_ = curr_bd;
//				return true;
//			}
//			prev_bd = curr_bd;
//			curr_bd = curr_bd->next_;
//		}
//
//		// this means the descriptor's block is larger than all elements in the list
//		prev_bd->next_ = bd;
//	}
//
//	return true;
//}
//
//void BlockAllocator::RemoveFromFreeList(BD* prev_bd, BD* curr_bd)
//{
//	if (prev_bd == NULL)
//	{
//		// this means we're removing the head
//		free_list_head_ = curr_bd->next_;
//	}
//	else
//	{
//		prev_bd->next_ = curr_bd->next_;
//	}
//
//	curr_bd->next_ = NULL;
//}
//
//bool BlockAllocator::AddToOutstandingList(BD* bd)
//{
//	ASSERT(bd != NULL);
//
//	// check if list is empty
//	if (outstanding_list_head_ == NULL)
//	{
//		// add the new descriptor to the front
//		bd->next_ = outstanding_list_head_;
//		outstanding_list_head_ = bd;
//		return true;
//	}
//
//	// if the new descriptor's block is smaller than the head's block, add it to the front
//	if (bd->block_size_ < outstanding_list_head_->block_size_)
//	{
//		bd->next_ = outstanding_list_head_;
//		outstanding_list_head_ = bd;
//	}
//	else
//	{
//		// add this descriptor based on ascending order of block size
//		BD* curr_bd = outstanding_list_head_;
//		BD* prev_bd = NULL;
//		while (curr_bd != NULL)
//		{
//			if (bd->block_size_ < curr_bd->block_size_)
//			{
//				if (prev_bd != NULL)
//				{
//					prev_bd->next_ = bd;
//				}
//				bd->next_ = curr_bd;
//				return true;
//			}
//			prev_bd = curr_bd;
//			curr_bd = curr_bd->next_;
//		}
//
//		// this means the descriptor's block is larger than all elements in the list
//		prev_bd->next_ = bd;
//	}
//
//	return true;
//}
//
//void BlockAllocator::RemoveFromOutstandingList(BD* prev_bd, BD* curr_bd)
//{
//	if (prev_bd == NULL)
//	{
//		// this means we're removing the head
//		outstanding_list_head_ = curr_bd->next_;
//	}
//	else
//	{
//		// this means we're removing an element in the middle/end
//		prev_bd->next_ = curr_bd->next_;
//	}
//
//	curr_bd->next_ = NULL;
//}

bool BlockAllocator::CheckMemoryOverwrite(BD* bd) const
{
	for (unsigned int i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		if (*(bd->block_pointer_ + i) != GUARDBAND_FILL || *(bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + bd->user_size_ + i) != GUARDBAND_FILL)
		{
			return true;
		}
	}
	return false;
}

void BlockAllocator::ClearBlock(BD* bd)
{
	for (unsigned int i = 0; i < bd->block_size_; ++i)
	{
		*(bd->block_pointer_ + i) = '*';
	}
	*(bd->block_pointer_) = '|';
	*(bd->block_pointer_ + bd->block_size_ - 1) = '|';
}

void* BlockAllocator::Alloc(const size_t size)
{
	// consider memory for guardbands
#ifdef BUILD_DEBUG
	size_t total_size = size + DEFAULT_GUARDBAND_SIZE * 2 + DEFAULT_BYTE_ALIGNMENT;
#else
	size_t total_size = size + DEFAULT_BYTE_ALIGNMENT;
#endif

	// declare a block descriptor to service this request
	BD* new_bd = NULL;

	// 1. look for a descriptor in the free list
	BD* free_bd = GetDescriptorFromFreeList(total_size);
	if (free_bd == NULL)
	{
		LOG_DEBUG("Insufficient memory!");
		return NULL;
	}

	// 2. check if we need to split the memory pointed by the descriptor returned from 1.
	if (free_bd->block_size_ <= total_size + MAX_EXTRA_MEMORY)
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
			// can't proceed without a new descriptor
			// so add the descriptor from 1 back to the free list and return NULL
			//AddToFreeList(free_bd);
			AddToList(&free_list_head_, &free_bd);
			LOG_DEBUG("Insufficient descriptors!");
			return NULL;
		}

		// calculate the address of the new block
		new_bd->block_pointer_ = free_bd->block_pointer_ + free_bd->block_size_ - total_size;
		new_bd->block_size_ = total_size;

		// adjust for byte alignment
		const unsigned int adjustment = ((uintptr_t)(const void*)(new_bd->block_pointer_)) % DEFAULT_BYTE_ALIGNMENT;
		new_bd->block_pointer_ += (adjustment > 0) ? (DEFAULT_BYTE_ALIGNMENT - adjustment) : 0;

		// splice the free block
		free_bd->block_size_ -= total_size;

		// add the remaining free block back to the free list
		//AddToFreeList(free_bd);
		AddToList(&free_list_head_, &free_bd);
	}

	// initialize the user pointer
	new_bd->user_pointer_ = new_bd->block_pointer_;
	new_bd->user_size_ = size;

#ifdef BUILD_DEBUG
	// add guardbands
	for (unsigned int i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		*(new_bd->block_pointer_ + i) = GUARDBAND_FILL;
		*(new_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + new_bd->user_size_ + i) = GUARDBAND_FILL;
	}

	// update the user pointer
	new_bd->user_pointer_ += DEFAULT_GUARDBAND_SIZE;
#endif

	// add the descriptor to the outstanding list
	//AddToOutstandingList(new_bd);
	AddToList(&outstanding_list_head_, &new_bd);

	// return a pointer to the user
	return new_bd->user_pointer_;
}

// Deallocate a block of memory
bool BlockAllocator::Free(void* pointer)
{
	bool free_successful = false;

	// validate input
	if (pointer == NULL)
	{
		LOG_DEBUG("Bad input provided to free!");
		return free_successful;
	}

	// search for the descriptor
	BD* curr_bd = outstanding_list_head_;
	BD* prev_bd = NULL;
	while (curr_bd != NULL)
	{
		if (curr_bd->user_pointer_ == pointer)
		{
			// check for overwrites
			if (CheckMemoryOverwrite(curr_bd))
			{
				LOG_DEBUG("Detected overwritten memory!");
			}

			// remove descriptor from outstanding list			
			//RemoveFromOutstandingList(prev_bd, curr_bd);
			RemoveFromList(&outstanding_list_head_, &prev_bd, &curr_bd);

			// clear the block
			ClearBlock(curr_bd);

			// add descriptor to free list
			//AddToFreeList(curr_bd);
			AddToList(&free_list_head_, &curr_bd);

			free_successful = true;
			break;
		}

		prev_bd = curr_bd;
		curr_bd = curr_bd->next_;
	}

	return free_successful;
}

// Run garbage collection
void BlockAllocator::Collect()
{}

// Query whether a given pointer is within this allocator's range
bool BlockAllocator::Contains(const void* pointer) const
{
	return ((unsigned char*)pointer >= block_ && (unsigned char*)pointer <= (block_ + total_block_size_));
}

// Query whether a given pointer is an outstanding allocation
bool BlockAllocator::IsAllocated(const void* pointer) const
{
	return false;
}

const size_t BlockAllocator::GetLargestFreeBlockSize() const
{
	size_t largest_size = 0;
	// loop the free list
	BD* bd = free_list_head_;
	while (bd != NULL)
	{
		// check actual block side not user block size
		if (bd->block_size_ > largest_size)
		{
			largest_size = bd->block_size_;
		}
		bd = bd->next_;
	}

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
	LOG_DEBUG("---------- %s ----------", __FUNCTION__);
	if (pool_head_ != NULL)
	{
		unsigned int count = 0;
		LOG_DEBUG("POOL:");
		for (BD* bd = pool_head_; bd != NULL; bd = bd->next_)
		{
			LOG_DEBUG("BD.id=%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG_DEBUG("POOL SIZE:%d", count);
	}

	if (free_list_head_ != NULL)
	{
		unsigned int count = 0;
		LOG_DEBUG("FREE:");
		for (BD* bd = free_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG_DEBUG("BD.id=%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG_DEBUG("FREE LIST SIZE:%d", count);
	}

	if (outstanding_list_head_ != NULL)
	{
		unsigned int count = 0;
		LOG_DEBUG("OUTSTANDING:");
		for (BD* bd = outstanding_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG_DEBUG("BD.id=%d size:%zu  ", bd->id_, bd->block_size_);
			++count;
		}
		LOG_DEBUG("OUTSTANDING LIST SIZE:%d", count);
	}
	LOG_DEBUG("---------- END ----------");
}
#endif

} // namespace engine