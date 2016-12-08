#include "BitArray.h"

namespace engine {
namespace data {

	inline bool BitArray::IsBitSet(size_t bit_index) const
	{
		// validate input
		ASSERT(bit_index < BIT_DEPTH);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		return (*(buckets_ + bucket_index) >> bit_index) & static_cast<size_t>(1);
	}

	inline bool BitArray::IsBitClear(size_t bit_index) const
	{
		return !IsBitSet(bit_index);
	}

} // namespace data
} // namespace engine