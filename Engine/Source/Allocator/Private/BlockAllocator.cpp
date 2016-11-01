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
	next_ = NULL;
	previous_ = NULL;
	block_pointer_ = NULL;
	block_size_ = 0;
#ifdef BUILD_DEBUG
	user_size_ = 0;
	id_ = BlockDescriptor::counter_++;
#endif
}

BlockAllocator* BlockAllocator::instance_ = NULL;

BlockAllocator::BlockAllocator() : block_(NULL),
	user_list_head_(NULL),
	free_list_head_(NULL),
	total_block_size_(DEFAULT_BLOCK_SIZE),
	size_of_BD_(sizeof(BD))
{
	Init();
}

BlockAllocator::BlockAllocator(const size_t block_size) : block_(NULL),
	user_list_head_(NULL),
	free_list_head_(NULL),
	total_block_size_(block_size),
	size_of_BD_(sizeof(BD))
{
	Init();
}

BlockAllocator::~BlockAllocator()
{
	_aligned_free(block_);
	block_ = NULL;
}

BlockAllocator* BlockAllocator::Create(const size_t block_size)
{
	if (BlockAllocator::instance_ == NULL)
	{
		BlockAllocator::instance_ = new BlockAllocator(block_size);
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
	const size_t adjustment = (total_block_size_ % DEFAULT_BYTE_ALIGNMENT);
	total_block_size_ += (adjustment > 0) ? DEFAULT_BYTE_ALIGNMENT - adjustment : 0;

	block_ = static_cast<uint8_t*>(_aligned_malloc(total_block_size_, DEFAULT_BYTE_ALIGNMENT));
#ifdef BUILD_DEBUG
	memset(block_, CLEAN_FILL, total_block_size_);
	LOG("Base start address:%p\tend address:%p", block_, (block_ + total_block_size_));
#endif

	InitFirstBlockDescriptor();
}

void BlockAllocator::InitFirstBlockDescriptor()
{
	// initialize the first descriptor
	BD* first_bd = reinterpret_cast<BD*>(block_);
	first_bd->Init();
	first_bd->block_pointer_ = block_ + size_of_BD_;
	first_bd->block_size_ = total_block_size_ - size_of_BD_;

	// add the descriptor to the free list
	AddToList(&free_list_head_, &first_bd, false);
}

void BlockAllocator::AddToList(BD** head, BD** bd, bool enable_sort)
{
	ASSERT(head != NULL);
	ASSERT(bd != NULL);

	// check if the list is empty
	if (*head == NULL)
	{
		// add the new descriptor to the front
		(*bd)->next_ = NULL;
		(*bd)->previous_ = NULL;
		*head = *bd;
		return;
	}

	// add the new descriptor to the front if:
	// sorting is disabled OR the new descriptor's block pointer is before the head's
	if (!enable_sort || (*bd)->block_pointer_ < (*head)->block_pointer_)
	{
		(*head)->previous_ = *bd;
		(*bd)->next_ = *head;
		(*bd)->previous_ = NULL;
		*head = *bd;
	}
	else if (enable_sort)
	{
		// add this descriptor based on ascending order of address
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
				(*bd)->previous_ = prev;
				curr->previous_ = *bd;
				return;
			}
			prev = curr;
			curr = curr->next_;
		}

		// this means the descriptor's block is larger than all elements in the list
		prev->next_ = *bd;
		(*bd)->next_ = NULL;
		(*bd)->previous_ = prev;
	}
}

void BlockAllocator::RemoveFromList(BD** head, BD** bd)
{
	ASSERT(head != NULL);
	ASSERT(bd != NULL);

	if ((*bd)->previous_ == NULL)
	{
		// this means we're removing the head
		*head = (*bd)->next_;
	}
	else
	{
		// update previous block's next pointer
		((*bd)->previous_)->next_ = (*bd)->next_;
	}

	// update next block's previous pointer
	if ((*bd)->next_)
	{
		((*bd)->next_)->previous_ = (*bd)->previous_;
	}

	// nullify current block's pointers
	(*bd)->previous_ = NULL;
	(*bd)->next_ = NULL;
}

#ifdef BUILD_DEBUG
bool BlockAllocator::CheckMemoryOverwrite(BD* bd) const
{
	ASSERT(bd != NULL);
	unsigned int lower_byte_counter = 0, upper_byte_counter = 0;
	for (unsigned int i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		// check lower guardband
		lower_byte_counter += (*(bd->block_pointer_ + i) == GUARDBAND_FILL) ? 1 : 0;

		// check upper guardband
		upper_byte_counter += (*(bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + bd->user_size_ + i) == GUARDBAND_FILL) ? 1 : 0;
	}

	bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
	if (found_overwrite)
	{
		LOG("Detected overwritten memory!");
	}

	return found_overwrite;
}

void BlockAllocator::ClearBlock(BD* bd, const unsigned char fill)
{
	ASSERT(bd != NULL);
	memset(bd->block_pointer_, fill, bd->block_size_);
}
#endif

void* BlockAllocator::Alloc(const size_t size)
{
	ASSERT(size > 0);

	// pre-calculate overhead required for an allocation
#ifdef BUILD_DEBUG
	const size_t overhead = DEFAULT_BYTE_ALIGNMENT + DEFAULT_GUARDBAND_SIZE * 2 + size_of_BD_;
#else
	const size_t overhead = DEFAULT_BYTE_ALIGNMENT + size_of_BD_;
#endif

	// declare a block descriptor to service this request
	BD* new_bd = NULL;

	// loop the free list for a descriptor to a block that is big enough
	bool did_defrag = false;
	BD* free_bd = free_list_head_;
	while (free_bd != NULL)
	{
		// check if this block is big enough
		if (size <= free_bd->block_size_)
		{
			// check if this block needs to be fragmented
			if (free_bd->block_size_ - size >= (MAX_EXTRA_MEMORY + size_of_BD_))
			{
				// if we need to split, calculate the address of the new block
				uint8_t* new_block_pointer = free_bd->block_pointer_ + free_bd->block_size_ - size - overhead;

				// adjust for byte alignment
				const uintptr_t adjustment = (reinterpret_cast<uintptr_t>(new_block_pointer)) % DEFAULT_BYTE_ALIGNMENT;
				new_block_pointer += (DEFAULT_BYTE_ALIGNMENT - adjustment);

				// initialize the new block's descriptor
				new_bd = reinterpret_cast<BD*>(new_block_pointer);
				new_bd->Init();
				new_bd->block_pointer_ = new_block_pointer + size_of_BD_;

				// reduce size by adjustment
				new_bd->block_size_ = size + overhead - size_of_BD_ - (DEFAULT_BYTE_ALIGNMENT - adjustment);

				// splice the free block
				free_bd->block_size_ -= (size_of_BD_ + new_bd->block_size_);

				// done with the search so break
				break;
			}
			else if (free_bd->block_size_ - size >= (DEFAULT_BYTE_ALIGNMENT + DEFAULT_GUARDBAND_SIZE * 2))
			{
				// if we don't need to split, use this free descriptor as is
				new_bd = free_bd;

				// remove it from the free list
				RemoveFromList(&free_list_head_, &new_bd);

				// done with the search so break
				break;
			}
		}

		// move to the next descriptor in the free list
		free_bd = free_bd->next_;

		// have we reached the end of the free list?
		if (free_bd == NULL)
		{
			// this means we still haven't found a free block that's big enough			
			if (!did_defrag)
			{
				// defragment but only ONCE
				did_defrag = true;
				Defragment();

				// start again at the front of the free list
				free_bd = free_list_head_;
			}
			else
			{
				LOG_ERROR("Insufficient memory!");
				return NULL;
			}
		}

	} // end of while loop to search for free blocks

	// this means we couldn't find a block even after defragmenting
	if (new_bd == NULL)
	{
		LOG_ERROR("Insufficient memory!");
		return NULL;
	}

#ifdef BUILD_DEBUG
	// clear the block
	ClearBlock(new_bd, CLEAN_FILL);

	// add guardbands
	for (unsigned int i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		*(new_bd->block_pointer_ + i) = GUARDBAND_FILL;
		*(new_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE + size + i) = GUARDBAND_FILL;
	}

	// save the size requested by the user for future use
	new_bd->user_size_ = size;
#endif

	// add the descriptor to the user list
	AddToList(&user_list_head_, &new_bd, false);

#ifdef BUILD_DEBUG
	return (new_bd->block_pointer_ + DEFAULT_GUARDBAND_SIZE);
#else
	return new_bd->block_pointer_;
#endif
}

// Deallocate a block of memory
bool BlockAllocator::Free(void* pointer)
{
	ASSERT(pointer != NULL);

	// calculate the address of the descriptor
#ifdef BUILD_DEBUG
	BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(pointer) - DEFAULT_GUARDBAND_SIZE - size_of_BD_);
#else
	BD* bd = reinterpret_cast<BD*>(static_cast<uint8_t*>(pointer) - size_of_BD_);
#endif

#ifdef BUILD_DEBUG
	// check for overwrites
	ASSERT(!CheckMemoryOverwrite(bd));
#endif

	// remove the descriptor from the user list
	RemoveFromList(&user_list_head_, &bd);

#ifdef BUILD_DEBUG
	// clear the block
	ClearBlock(bd, DEAD_FILL);

	// reset the user size
	bd->user_size_ = 0;
#endif

	// add the descriptor to the free list
	AddToList(&free_list_head_, &bd, true);

	return true;
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
		// check if next block is contiguous to current block
		if (curr->block_pointer_ + curr->block_size_ == curr->next_->block_pointer_ - size_of_BD_)
		{
			BD* next_bd = curr->next_;

#ifdef BUILD_DEBUG
			LOG("Merging %zu bytes from block-%d & block-%d", (next_bd->block_size_ + size_of_BD_), curr->id_, next_bd->id_);
#endif
			// update current descriptor's block size
			curr->block_size_ += (next_bd->block_size_ + size_of_BD_);

#ifdef BUILD_DEBUG
			bytes_combined += (next_bd->block_size_ + size_of_BD_);
#endif
			// update current descriptor's next & previous pointers
			curr->next_ = next_bd->next_;
			if (next_bd->next_)
			{
				(next_bd->next_)->previous_ = curr;
			}

			// clear next descriptor
			next_bd->next_ = NULL;
			next_bd->previous_ = NULL;
			next_bd->block_pointer_ = NULL;
			next_bd->block_size_ = 0;

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
	return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) <= (block_ + total_block_size_));
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

#ifdef BUILD_DEBUG
	return largest_size - DEFAULT_BYTE_ALIGNMENT - DEFAULT_GUARDBAND_SIZE * 2;
#else
	return largest_size - DEFAULT_BYTE_ALIGNMENT;
#endif
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
	PrintFreeDescriptors();
	PrintUsedDescriptors();	
}

void BlockAllocator::PrintFreeDescriptors() const
{
	LOG("---------- %s ----------", __FUNCTION__);
	if (free_list_head_ != NULL)
	{
		unsigned int count = 0;
		LOG("FREE:");
		for (BD* bd = free_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG("BD.id:%d size:%zu", bd->id_, bd->block_size_);
			++count;
		}
		LOG("FREE LIST SIZE:%d", count);
	}
	else
	{
		LOG("FREE LIST EMPTY!");
	}
	LOG("---------- END ----------");
}

void BlockAllocator::PrintUsedDescriptors() const
{
	LOG("---------- %s ----------", __FUNCTION__);
	if (user_list_head_ != NULL)
	{
		unsigned int count = 0;
		LOG("USER:");
		for (BD* bd = user_list_head_; bd != NULL; bd = bd->next_)
		{
			LOG("BD.id:%d size:%zu", bd->id_, bd->block_size_);
			++count;
		}
		LOG("USER LIST SIZE:%d", count);
	}
	else
	{
		LOG("USER LIST EMPTY!");
	}
	LOG("---------- END ----------");
}
#endif

} // namespace engine