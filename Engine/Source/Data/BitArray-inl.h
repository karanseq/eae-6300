#include "BitArray.h"

// library includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Memory\AllocatorOverrides.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace data {

	inline BitArray& BitArray::operator=(const BitArray& bit_array)
	{
		if (this != &bit_array)
		{
			SAFE_DELETE_ARRAY(buckets_);

			allocator_ = bit_array.allocator_;
			
			buckets_ = new (allocator_) size_t[bit_array.num_buckets_];
			memcpy(buckets_, bit_array.buckets_, bit_array.num_buckets_);
			
			num_buckets_ = bit_array.num_buckets_;
			num_bits_ = bit_array.num_bits_;
		}
		return *this;
	}

	inline BitArray& BitArray::operator=(BitArray&& bit_array)
	{
		if (this != &bit_array)
		{
			allocator_ = bit_array.allocator_;
			std::swap(buckets_, bit_array.buckets_);
			num_buckets_ = bit_array.num_buckets_;
			num_bits_ = bit_array.num_bits_;
		}
		return *this;
	}

	inline void BitArray::ClearAll()
	{
		memset(buckets_, 0, sizeof(buckets_) * num_buckets_);
	}

	inline void BitArray::SetAll()
	{
		memset(buckets_, ~0, sizeof(buckets_) * num_buckets_);
	}

	inline bool BitArray::IsBitSet(size_t bit_index) const
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		return (*(buckets_ + bucket_index) >> bit_index) & static_cast<size_t>(1);
	}

	inline bool BitArray::IsBitClear(size_t bit_index) const
	{
		return !IsBitSet(bit_index);
	}

	inline bool BitArray::operator[](size_t bit_index) const
	{
		return IsBitSet(bit_index);
	}

} // namespace data
} // namespace engine