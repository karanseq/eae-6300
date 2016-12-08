#include <stdint.h>

#include "Logger\Logger.h"
#include "Assert\Assert.h"
#include "Memory\BlockAllocator.h"
#include "Data\BitArray.h"

#define BIT_DEPTH 32

void TestBitArray()
{
	LOG("-------------------- Running Bit Array Test --------------------");

	engine::memory::BlockAllocator* allocator = engine::memory::BlockAllocator::GetDefaultAllocator();

	{
		engine::data::BitArray bit_array(63, true, allocator);

		bit_array.SetBit(9);
		bool is_set = bit_array.IsBitSet(9);
		bool is_clear = bit_array.IsBitClear(9);

		bit_array.ClearBit(9);
		is_set = bit_array.IsBitSet(9);
		is_clear = bit_array.IsBitClear(9);

		size_t bit_index = 0;
		bit_array.GetFirstSetBit(bit_index);
	}


	engine::memory::BlockAllocator::DestroyDefaultAllocator();

	LOG("-------------------- Finished Bit Array Test --------------------");
}
