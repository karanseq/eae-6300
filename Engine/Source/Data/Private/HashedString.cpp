#include "Data\HashedString.h"

// library includes
#include <string.h>

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace data {

HashedString::HashedString(const char* i_string) : hash_(Hash(i_string))
{}

HashedString::HashedString(const HashedString& i_copy) : hash_(i_copy.hash_)
{}

HashedString::HashedString(const PooledString& i_string) : hash_(Hash(i_string.GetString()))
{}

unsigned int HashedString::Hash(const char* i_string)
{
	ASSERT(i_string);
	return Hash(reinterpret_cast<const void*>(i_string), strlen(i_string));
}

unsigned int HashedString::Hash(const void* i_bytes, unsigned int i_byte_count)
{
	// FNV hash, http://isthe.com/chongo/tech/comp/fnv

	register const unsigned char* p = static_cast<const unsigned char*>(i_bytes);
	unsigned int hash = 2166136261;

	for (unsigned int i = 0; i < i_byte_count; ++i)
	{
		hash = 16777619 * (hash ^ p[i]);
	}

	return hash ^ (hash >> 16);
}

} // namespace data
} // namespace engine
