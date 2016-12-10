#include "Memory\FixedSizeAllocator.h"

// engine includes
#include "Memory\AllocatorUtil.h"
#include "Data\BitArray.h"
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace memory {

FixedSizeAllocator*			FixedSizeAllocator::registered_allocators_[MAX_FIXED_SIZE_ALLOCATORS] = { nullptr };

#ifdef BUILD_DEBUG
uint8_t						FixedSizeAllocator::counter_ = 0;
#endif

FixedSizeAllocator::FixedSizeAllocator(void* memory, size_t total_block_size, size_t fixed_block_size, size_t num_blocks, BlockAllocator* allocator) : block_(static_cast<uint8_t*>(memory)),
	total_block_size_(total_block_size),
	fixed_block_size_(fixed_block_size),
	num_blocks_(num_blocks),
	block_allocator_(allocator),
	block_state_(nullptr)
{
	ASSERT(block_);
	ASSERT(fixed_block_size_ > 0);
	ASSERT(num_blocks_ > 0);
	ASSERT(block_allocator_);

	// get the amount of memory required to create a bit array
	const size_t bit_array_memory_size = engine::data::BitArray::GetRequiredMemorySize(num_blocks);	

	// create the bit array at the end of the block
	uint8_t* bit_array_memory = block_ + total_block_size_ - bit_array_memory_size;
	block_state_ = engine::data::BitArray::Create(num_blocks_, bit_array_memory);
	
	// reduce the usable memory
	total_block_size_ -= bit_array_memory_size;

#ifdef BUILD_DEBUG
	id_ = FixedSizeAllocator::counter_++;
	memset(block_, CLEAN_FILL, total_block_size_);
	LOG("FixedSizeAllocator-%d created with %zu blocks of size:%zu", id_, num_blocks_, fixed_block_size_);
#endif
}

FixedSizeAllocator* FixedSizeAllocator::Create(size_t block_size, size_t num_blocks, BlockAllocator* allocator)
{
	ASSERT(block_size);
	ASSERT(num_blocks);
	ASSERT(allocator);

#ifdef BUILD_DEBUG
	const size_t guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
	const size_t guardband_size = 0;
#endif

	// calculate the amount of memory required to create an FSA
	size_t fsa_memory_size = sizeof(FixedSizeAllocator) + num_blocks * (block_size + guardband_size * 2) + engine::data::BitArray::GetRequiredMemorySize(num_blocks);

	// allocate memory
	void* memory = allocator->Alloc(fsa_memory_size);
	ASSERT(memory);

	// move up the address of the usable block
	uint8_t* fsa_memory = static_cast<uint8_t*>(memory);
	fsa_memory += sizeof(FixedSizeAllocator);
	fsa_memory_size -= sizeof(FixedSizeAllocator);

	// create the FSA
	FixedSizeAllocator* fsa = new (memory) FixedSizeAllocator(fsa_memory, fsa_memory_size, block_size, num_blocks, allocator);
	ASSERT(fsa);

	return fsa;
}

void FixedSizeAllocator::Destroy(FixedSizeAllocator* allocator)
{
	ASSERT(allocator);

	// TODO: print diagnostics

	BlockAllocator* block_allocator = allocator->block_allocator_;
	uint8_t id = allocator->id_;
	block_allocator->Free(allocator);

	LOG("FixedSizeAllocator-%d destroyed", id);
}

#ifdef BUILD_DEBUG
bool FixedSizeAllocator::CheckMemoryOverwrite(size_t bit_index) const
{
	ASSERT(bit_index >= 0 && bit_index < num_blocks_);

	uint8_t* block = GetPointerForBlock(bit_index);

	uint8_t lower_byte_counter = 0, upper_byte_counter = 0;
	for (uint8_t i = 0; i < DEFAULT_GUARDBAND_SIZE; ++i)
	{
		// check lower guardband
		lower_byte_counter += (*(block + i) == GUARDBAND_FILL) ? 1 : 0;

		// check upper guardband
		upper_byte_counter += (*(block + DEFAULT_GUARDBAND_SIZE + fixed_block_size_ + i) == GUARDBAND_FILL) ? 1 : 0;
	}

	bool found_overwrite = !(lower_byte_counter >= DEFAULT_GUARDBAND_SIZE && upper_byte_counter >= DEFAULT_GUARDBAND_SIZE);
	if (found_overwrite)
	{
		LOG_ERROR("Detected overwritten memory!");
	}

	return found_overwrite;
}
#endif

void* FixedSizeAllocator::Alloc()
{
	// check if there are any blocks available
	size_t bit_index = -1;
	bool block_available = block_state_->GetFirstClearBit(bit_index);
	
	// return nullptr if nothing was available
	if (!block_available)
	{
		LOG_ERROR("FixedSizeAllocator-%d with block_size=%zu ran out of memory!", id_, fixed_block_size_);
		return nullptr;
	}

	// set the bit at this index
	block_state_->SetBit(bit_index);

#ifdef BUILD_DEBUG
	const size_t	guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
	const size_t	guardband_size = 0;
#endif

	// calculate the block's address
	uint8_t* block = block_ + bit_index * (DEFAULT_GUARDBAND_SIZE * 2 + fixed_block_size_);

#ifdef BUILD_DEBUG
	// clear the block
	memset(block, CLEAN_FILL, fixed_block_size_);

	// add guardbands
	for (uint8_t i = 0; i < guardband_size; ++i)
	{
		*(block + i) = GUARDBAND_FILL;
		*(block + guardband_size + fixed_block_size_ + i) = GUARDBAND_FILL;
	}
#endif

	return (block + guardband_size);
}

bool FixedSizeAllocator::Free(void* pointer)
{
	ASSERT(pointer != nullptr);

	// return if this allocator does not contain this pointer
	if (!Contains(pointer))
	{
		return false;
	}

#ifdef BUILD_DEBUG
	const size_t	guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
	const size_t	guardband_size = 0;
#endif

	// get a pointer to a block
	uint8_t* block = static_cast<uint8_t*>(pointer) - guardband_size;

	// check if we recognize this pointer
	if ((block - block_) & ((fixed_block_size_ + guardband_size) - 1))
	{
		LOG_ERROR("FixedSizeAllocator-%d could not find pointer=%p passed into Free...bad adress!", id_, pointer);
		return false;
	}

	// calculate the index of the bit that represents this block
	size_t bit_index = (block - block_) / (fixed_block_size_ + guardband_size);

	// validate bit_index
	if (bit_index < 0 || bit_index >= num_blocks_)
	{
		LOG_ERROR("FixedSizeAllocator-%d could not find pointer=%p passed into Free...couldn't map to an index!", id_, pointer);
		return false;
	}

	// check if this block is currently allocated
	if (block_state_->IsBitClear(bit_index))
	{
		LOG_ERROR("FixedSizeAllocator-%d could not free pointer=%p since it is not currently allocated!", id_, pointer);
		return false;
	}

#ifdef BUILD_DEBUG
	// check for overwrites
	ASSERT(!CheckMemoryOverwrite(bit_index));

	// clear the block
	memset(block, DEAD_FILL, fixed_block_size_ + guardband_size * 2);
#endif

	// clear the bit at this index
	block_state_->ClearBit(bit_index);

	return true;
}

bool FixedSizeAllocator::IsAllocated(const void* pointer) const
{
	ASSERT(pointer != nullptr);

	// return if this allocator does not contain this pointer
	if (!Contains(pointer))
	{
		return false;
	}

#ifdef BUILD_DEBUG
	const size_t	guardband_size = DEFAULT_GUARDBAND_SIZE;
#else
	const size_t	guardband_size = 0;
#endif

	// get a pointer to a block
	const uint8_t* block = static_cast<const uint8_t*>(pointer) - guardband_size;

	// check if we recognize this pointer
	if ((block - block_) & ((fixed_block_size_ + guardband_size) - 1))
	{
		return false;
	}

	// calculate the index of the bit that represents this block
	size_t bit_index = (block - block_) / (fixed_block_size_ + guardband_size);

	// validate bit_index
	if (bit_index < 0 || bit_index >= num_blocks_)
	{
		return false;
	}

	// check if this block is currently allocated
	if (block_state_->IsBitSet(bit_index))
	{
		return false;
	}

	return true;
}

} // namespace memory
} // namespace engine