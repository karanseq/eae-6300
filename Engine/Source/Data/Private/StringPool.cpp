#include "Data\StringPool.h"

// library includes
#include <new> // for placement new
#include <string.h>

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"

namespace engine {
namespace data {

// static member initialization
StringPool* StringPool::instance_ = nullptr;
const size_t StringPool::DEFAULT_POOL_SIZE = 256 * 1024;

StringPool::StringPool(uint8_t* i_pool, size_t i_pool_size) : pool_(i_pool),
	pool_size_(i_pool_size)
{
	ASSERT(pool_ != nullptr);
	ASSERT(pool_size_ > 0);

	*pool_ = static_cast<size_t>(0);
}

StringPool::~StringPool()
{

}

StringPool* StringPool::Create(size_t i_bytes)
{
	ASSERT(i_bytes > 0);

	if (StringPool::instance_ == nullptr)
	{
		// allocate memory for an instance of the string pool class
		const size_t total_memory_size = sizeof(StringPool) + i_bytes;
		uint8_t* total_memory = static_cast<uint8_t*>(engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(total_memory_size));
		ASSERT(total_memory != nullptr);

		// create a new instance in place
		StringPool::instance_ = new (total_memory) StringPool(total_memory + sizeof(StringPool), i_bytes);
	}
	return StringPool::instance_;
}

void StringPool::Destroy()
{
	SAFE_DELETE(StringPool::instance_);
}

const char* StringPool::Add(const char* i_string)
{
	ASSERT(i_string != nullptr);

	// check if the string exists in the pool
	const char* string = Find(i_string);
	if (string)
	{
		return string;
	}

	// loop to the end of the pool
	size_t string_length = static_cast<size_t>(*pool_);
	uint8_t* pool_pointer = pool_;
	while (string_length > 0)
	{
		pool_pointer += (sizeof(size_t) + string_length);
		string_length = static_cast<size_t>(*pool_pointer);
	}

	// calculate remaining memory
	const size_t remaining_memory = static_cast<size_t>(pool_ + pool_size_ - pool_pointer);

	// check if we have enough memory for this string and another size_t 
	const size_t input_string_length = strlen(i_string) + 1;
	ASSERT(input_string_length + sizeof(size_t) * 2 <= remaining_memory);

	// save the size of this string
	size_t* string_size = reinterpret_cast<size_t*>(pool_pointer);
	*string_size = input_string_length;

	// copy the string
	memcpy_s((pool_pointer + sizeof(size_t)), remaining_memory - sizeof(size_t), i_string, input_string_length);

	// add null-termination
	*(pool_pointer + sizeof(size_t) + input_string_length - 1) = '\0';

	// add a size_t to the end of this string
	size_t* next_string_size_t = reinterpret_cast<size_t*>(pool_pointer + sizeof(size_t) + input_string_length);
	*next_string_size_t = 0;

	return reinterpret_cast<const char*>(pool_pointer + sizeof(size_t));
}

const char* StringPool::Find(const char* i_string)
{
	ASSERT(i_string != nullptr);

	// get the size of the first string in the pool
	size_t string_length = static_cast<size_t>(*pool_);

	// loop the pool in search of the input string
	uint8_t* pool_pointer = pool_;
	while (string_length > 0)
	{
		// get a pointer to the string
		char* string = reinterpret_cast<char*>(pool_pointer + sizeof(size_t));

		// compare the string with the input
		if (strcmp(i_string, string) == 0)
		{
			return string;
		}

		// move to the next string
		pool_pointer += string_length + sizeof(size_t);
		string_length = static_cast<size_t>(*pool_pointer);
	}

	return nullptr;
}

} // namespace data
} // namespace engine
