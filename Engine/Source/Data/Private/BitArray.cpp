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

	BitArray::BitArray(size_t num_bits, bool start_set, engine::memory::BlockAllocator* allocator) : allocator_(allocator),
		buckets_(nullptr),
		num_buckets_(((num_bits & (BIT_DEPTH - 1)) ? 1 : 0) + num_bits / BIT_DEPTH),
		num_bits_(num_bits)
	{
		ASSERT(allocator_);

		// allocate required memory
		buckets_ = new (allocator_) size_t[num_buckets_];

		// start with set or clear
		memset(buckets_, start_set ? ~0 : 0, sizeof(buckets_) * num_buckets_);
	}

	BitArray::~BitArray()
	{
		SAFE_DELETE_ARRAY(buckets_);
	}

	BitArray::BitArray(const BitArray& copy) : allocator_(copy.allocator_),
		buckets_(new (allocator_) size_t[copy.num_buckets_]),
		num_buckets_(copy.num_buckets_),
		num_bits_(copy.num_bits_)
	{
		memcpy(buckets_, copy.buckets_, sizeof(buckets_) * num_buckets_);
	}

	BitArray::BitArray(BitArray&& copy) : allocator_(copy.allocator_),
		buckets_(copy.buckets_),
		num_buckets_(copy.num_buckets_),
		num_bits_(copy.num_bits_)
	{
		copy.buckets_ = nullptr;
	}

	bool BitArray::AreAllClear() const
	{
		size_t bit_index = 0;
		bool success = GetFirstSetBit(bit_index);
		
		// if we found no set bits at all, return true
		// if we found a set bit, return true if it was the last bit else return false
		return (!success || bit_index >= num_bits_);
	}

	bool BitArray::AreAllSet() const
	{
		size_t bit_index = 0;
		bool success = GetFirstClearBit(bit_index);

		// if we found no clear bits at all, return true
		// if we found a clear bit, return true if it was the last bit else return false
		return (!success || bit_index >= num_bits_);
	}

	void BitArray::SetBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		// set the respective bit in the respective bucket
		*(buckets_ + bucket_index) |= static_cast<size_t>(1) << bit_index;
	}

	void BitArray::ClearBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		// set the respective bit in the respective bucket
		*(buckets_ + bucket_index) &= ~(static_cast<size_t>(1) << bit_index);
	}

	void BitArray::ToggleBit(size_t bit_index)
	{
		// validate input
		ASSERT(bit_index >= 0);
		ASSERT(bit_index < num_bits_);

		// calculate the bucket index
		size_t bucket_index = bit_index / BIT_DEPTH;

		*(buckets_ + bucket_index) ^= static_cast<size_t>(1) << bit_index;
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

#if defined(_WIN64)
		char is_non_zero = _BitScanForward64(&bit_index_long, buckets_[bucket_index]);
#else
		char is_non_zero = _BitScanForward(&bit_index_long, buckets_[bucket_index]);
#endif
		if (is_non_zero)
		{
			bit_index = bit_index_long;
			return true;
		}

		return false;
	}

	bool BitArray::GetFirstClearBit(size_t &bit_index) const
	{
		size_t bucket_index = 0;
		unsigned long bit_index_long = 0;

		// quick skip buckets where no bits are clear
		while (buckets_[bucket_index] == ~0 && bucket_index < num_buckets_)
		{
			++bucket_index;
		}

		// were all bits set?
		if (bucket_index >= num_buckets_)
		{
			return false;
		}

#if defined(_WIN64)
		char is_non_zero = _BitScanForward64(&bit_index_long, ~(buckets_[bucket_index]));
#else
		char is_non_zero = _BitScanForward(&bit_index_long, ~(buckets_[bucket_index]));
#endif
		if (is_non_zero)
		{
			bit_index = bit_index_long;
			return true;
		}

		return false;
	}	

} // namespace data
} // namespace engine
