#include "BlockAllocator.h"

// library includes
#include <string.h>			// for memset

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace memory {

	inline size_t BlockAllocator::GetSizeOfBD()
	{
		return size_of_BD_;
	}

#ifdef BUILD_DEBUG
	inline void BlockAllocator::ClearBlock(BD* i_bd, const unsigned char i_fill)
	{
		ASSERT(i_bd != nullptr);
		memset(i_bd->block_pointer, i_fill, i_bd->block_size);
	}

	inline unsigned int BlockAllocator::GetID() const
	{
		return id_;
	}
#endif

	// Query whether a given pointer is within this allocator's range
	inline bool BlockAllocator::Contains(const void* i_pointer) const
	{
		ASSERT(i_pointer != nullptr);
		return (static_cast<const uint8_t*>(i_pointer) >= block_ && static_cast<const uint8_t*>(i_pointer) <= (block_ + total_block_size_));
	}

} // namespace memory
} // namespace engine