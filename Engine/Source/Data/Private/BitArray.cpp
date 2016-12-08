#include "Data\BitArray.h"

// library includes
#include <string.h>
#include <intrin.h>

// engine includes
#include "Assert\Assert.h"
#include "Memory\AllocatorOverrides.h"
#include "Memory\AllocatorUtil.h"

namespace engine {
namespace data {

	BitArray::BitArray(size_t num_bits, bool start_set, engine::memory::BlockAllocator* allocator) : buckets_(nullptr),
		num_buckets_(1 + num_bits / BIT_DEPTH),
		num_bits_(num_bits)
	{
		ASSERT(allocator);

		// allocate required memory
		buckets_ = new (allocator) size_t(num_buckets_);

		// start with set or clear
		memset(buckets_, start_set ? ~0 : 0, num_buckets_);
	}

	BitArray::~BitArray()
	{
		SAFE_DELETE(buckets_);
	}

	void BitArray::ClearAll()
	{
		for (size_t i = 0; i < num_buckets_; ++i)
		{
			buckets_[i] = 0;
		}
	}

	void BitArray::SetAll()
	{
		for (size_t i = 0; i < num_buckets_; ++i)
		{
			buckets_[i] = ~0;
		}
	}

	bool BitArray::AreAllClear() const
	{
		return false;
	}

	bool BitArray::AreAllSet() const
	{
		return false;
	}

	void BitArray::SetBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index < BIT_DEPTH);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		// set the respective bit in the respective bucket
		*(buckets_ + bucket_index) |= static_cast<size_t>(1) << bit_index;
	}

	void BitArray::ClearBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index < BIT_DEPTH);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		// set the respective bit in the respective bucket
		*(buckets_ + bucket_index) &= ~(static_cast<size_t>(1) << bit_index);
	}

	bool BitArray::GetFirstSetBit(size_t &bit_index) const
	{
		size_t bucket_index = 0;
		unsigned long bit_index_long = 0;

		// quick skip buckets where no bits are set
		while (buckets_[bucket_index] == 0 && bucket_index < num_buckets_)
		{
			++bucket_index;
		}

		// were all bits clear?
		if (bucket_index >= num_buckets_)
		{
			return false;
		}

		char is_non_zero = _BitScanForward(&bit_index_long, buckets_[bucket_index]);
		if (is_non_zero)
		{
			bit_index = bit_index_long;
			return true;
		}

		return false;
	}

	bool BitArray::GetFirstClearBit(size_t &bit_index) const
	{
		return false;
	}

} // namespace data
} // namespace engine
