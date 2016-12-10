#include "FixedSizeAllocator.h"
#include "Assert\Assert.h"

#include <string.h>			// for memset

namespace engine {
namespace memory {

	inline FixedSizeAllocator** const FixedSizeAllocator::GetAvailableFixedSizeAllocators()
	{
		return available_allocators_;
	}

	inline uint8_t* FixedSizeAllocator::GetPointerForBlock(size_t bit_index) const
	{
		ASSERT(bit_index >= 0 && bit_index < num_blocks_);
#ifdef BUILD_DEBUG
		return block_ + (bit_index * (DEFAULT_GUARDBAND_SIZE * 2 + sizeof(size_t) + fixed_block_size_));
#else
		return block_ + (bit_index * fixed_block_size_);
#endif
	}

#ifdef BUILD_DEBUG
	inline void FixedSizeAllocator::ClearBlock(size_t bit_index, const unsigned char fill)
	{
		ASSERT(bit_index >= 0 && bit_index < num_blocks_);
		uint8_t* block = GetPointerForBlock(bit_index);
		memset(block, fill, fixed_block_size_);
	}

	inline unsigned int FixedSizeAllocator::GetID() const
	{
		return id_;
	}
#endif

	// Query whether a given pointer is within this allocator's range
	inline bool FixedSizeAllocator::Contains(const void* pointer) const
	{
		ASSERT(pointer != nullptr);
		return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) <= (block_ + total_block_size_));
	}

	inline size_t FixedSizeAllocator::GetBlockSize() const
	{
		return fixed_block_size_;
	}

	inline size_t FixedSizeAllocator::GetNumBlocks() const
	{
		return num_blocks_;
	}

} // namespace memory
} // namespace engine