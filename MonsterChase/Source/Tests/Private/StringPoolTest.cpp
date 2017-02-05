// library includes
#include <stdlib.h>
#include <vector>

// engine includes
#include "Assert\Assert.h"
#include "Data\StringPool.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"

char* MakeRandomWord(size_t i_length)
{
	ASSERT(i_length > 0);

	char* word = reinterpret_cast<char*>(engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(i_length + 1));
	for (size_t i = 0; i < i_length; ++i)
	{
		*(word + i) = 65 + rand() % 26;
	}
	*(word + i_length - 1) = '\0';

	return word;
}

void TestStringPool()
{
	LOG("-------------------- Running StringPool Test --------------------");

	const char* search_string = "sentient";
	const char* added_string = engine::data::StringPool::Get()->Add(search_string);
	const char* found_string = engine::data::StringPool::Get()->Find(search_string);
	found_string = engine::data::StringPool::Get()->Find(added_string);
	ASSERT(found_string == added_string);

	added_string = engine::data::StringPool::Get()->Add("transcendant");
	found_string = engine::data::StringPool::Get()->Find("transcendant");
	found_string = engine::data::StringPool::Get()->Find(added_string);
	ASSERT(found_string == added_string);
	
	LOG("-------------------- Finished StringPool Test --------------------");
}