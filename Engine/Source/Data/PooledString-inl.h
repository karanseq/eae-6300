#include "PooledString.h"

namespace engine {
namespace data {

inline PooledString& PooledString::operator=(const PooledString& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		string_ = i_copy.string_;
	}
	return *this;
}

inline bool PooledString::operator==(const PooledString& i_other) const
{
	return string_ == i_other.string_;
}

inline const char* PooledString::GetString() const
{
	return string_;
}

} // namespace data
} // namesapce engine
