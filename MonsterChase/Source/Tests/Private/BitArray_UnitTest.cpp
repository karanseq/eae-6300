#include "Data\BitArray.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"

void BitArray_UnitTest(void)
{
	LOG("-------------------- Running BitArray_UnitTest --------------------");

	using namespace engine::data;

	const size_t bitCount = 1000;

	const size_t bit_array_memory_size = BitArray::GetRequiredMemorySize(bitCount);
	void* bit_array_memory = engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(bit_array_memory_size);

	BitArray* pMyArray = BitArray::Create(bitCount, bit_array_memory);

	pMyArray->SetBit(20);

	size_t firstSetBit = 0;
	size_t firstClearBit = 0;

	bool foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
	assert(foundSetBit && (firstSetBit == 20));

	pMyArray->ClearBit(20);
	foundSetBit = pMyArray->GetFirstSetBit(firstSetBit);
	assert(foundSetBit == false);

	for (uint16_t i = 0; i < bitCount; i++)
	{
		assert(pMyArray->IsBitClear(i) == true);
		assert(pMyArray->IsBitSet(i) == false);

		size_t bit = 0;

		pMyArray->GetFirstClearBit(bit);
		assert(bit == i);

		pMyArray->SetBit(i);

		assert(pMyArray->IsBitClear(i) == false);
		assert(pMyArray->IsBitSet(i) == true);

		bool success = pMyArray->GetFirstClearBit(bit);
		assert(((i < (bitCount - 1)) && success && (bit == (i + 1))) || ((i == (bitCount - 1)) && !success));
	}

	//pMyArray->SetAllBits();
	pMyArray->SetAll();
	assert(pMyArray->GetFirstClearBit(firstClearBit) == false);

	//pMyArray->ClearAllBits();
	pMyArray->ClearAll();
	assert(pMyArray->GetFirstSetBit(firstSetBit) == false);

	//delete pMyArray;
	engine::memory::BlockAllocator::GetDefaultAllocator()->Free(pMyArray);

	LOG("-------------------- Finished BitArray_UnitTest --------------------");
}