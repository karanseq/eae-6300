#include "HashedString.h"

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

} // namespace data
} // namespace engine
