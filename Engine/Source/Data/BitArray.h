#ifndef ENGINE_BIT_ARRAY_H_
#define ENGINE_BIT_ARRAY_H_

#include "Memory\BlockAllocator.h"

namespace engine {
namespace data {

#if defined(_WIN64)
	typedef uint64_t			uintx_t;
	#define BIT_DEPTH			64
#else
	typedef uint32_t			uintx_t;
	#define BIT_DEPTH			32
#endif

	class BitArray
	{
	private:
		// TODO: add copy & move constructors & assignment operators
		BitArray(const BitArray& copy);
		BitArray& operator=(const BitArray& bit_array);
	
	public:
		//static BitArray* Create(size_t num_bits, bool start_clear = true, engine::memory::BlockAllocator* allocator = engine::memory::BlockAllocator::GetDefaultAllocator());
		BitArray(size_t num_bits, bool start_set = false, engine::memory::BlockAllocator* allocator = engine::memory::BlockAllocator::GetDefaultAllocator());
		~BitArray();

		void ClearAll();
		void SetAll();

		bool AreAllClear() const;
		bool AreAllSet() const;

		inline bool IsBitSet(size_t bit_index) const;
		inline bool IsBitClear(size_t bit_index) const;

		void SetBit(size_t bit_index);
		void ClearBit(size_t bit_index);

		bool GetFirstSetBit(size_t &bit_index) const;
		bool GetFirstClearBit(size_t &bit_index) const;

		bool operator[](size_t bit_index) const;

	private:
		size_t*							buckets_;
		size_t							num_buckets_;
		size_t							num_bits_;

	}; // class BitArray

} // namespace data
} // namespace engine

#include "BitArray-inl.h"

#endif // ENGINE_BIT_ARRAY_H_