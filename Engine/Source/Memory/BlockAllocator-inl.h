#include "BlockAllocator.h"
#include "Assert\Assert.h"

#include <string.h>			// for memset

namespace engine
{
	inline size_t BlockAllocator::GetSizeOfBD()
	{
		return size_of_BD_;
	}

#ifdef BUILD_DEBUG
	inline void BlockAllocator::ClearBlock(BD* bd, const unsigned char fill)
	{
		ASSERT(bd != nullptr);
		memset(bd->block_pointer_, fill, bd->block_size_);
	}

	inline unsigned int BlockAllocator::GetID() const
	{
		return id_;
	}
#endif

	// Query whether a given pointer is within this allocator's range
	inline bool BlockAllocator::Contains(const void* pointer) const
	{
		ASSERT(pointer != nullptr);
		return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) <= (block_ + total_block_size_));
	}

} // namespace engine