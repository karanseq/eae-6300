#include "BlockAllocator.h"
#include "Logger\Logger.h"

#include <string.h>			// for memset

namespace engine
{
#ifdef BUILD_DEBUG
	inline void BlockAllocator::ClearBlock(const BD* bd, const unsigned char fill)
	{
		ASSERT(bd != NULL);
		memset(bd->block_pointer_, fill, bd->block_size_);
	}
#endif

	// Query whether a given pointer is within this allocator's range
	inline bool BlockAllocator::Contains(const void* pointer) const
	{
		ASSERT(pointer != NULL);
		return (static_cast<const uint8_t*>(pointer) >= block_ && static_cast<const uint8_t*>(pointer) <= (block_ + total_block_size_));
	}

} // namespace engine