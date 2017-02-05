#include "HashedString.h"

// library includes
#include <string.h>

// engine includes
#include "Assert\Assert.h"

namespace engine {
namespace data {

inline HashedString& HashedString::operator=(const HashedString& i_copy)
{
	if (this != &i_copy)
	{
		hash_ = i_copy.hash_;
	}
	return *this;
}

inline bool HashedString::operator==(const HashedString& i_copy) const
{
	return (hash_ == i_copy.hash_);
}

inline unsigned int HashedString::GetHash() const
{
	return hash_;
}

inline unsigned int HashedString::Hash(const char* i_string)
{
	ASSERT(i_string);
	return Hash(reinterpret_cast<const void*>(i_string), strlen(i_string));
}

} // namespace data
} // namespace engine
