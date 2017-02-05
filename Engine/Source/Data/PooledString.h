#ifndef POOLED_STRING_H_
#define POOLED_STRING_H_

// engine includes
#include "StringPool.h"

namespace engine {
namespace data {

class PooledString
{
public:
	explicit PooledString(const char* i_string) : string_(StringPool::Get()->Add(i_string))
	{}
	~PooledString()
	{}

	PooledString(const PooledString& i_copy) : string_(i_copy.string_)
	{}

	inline PooledString& operator=(const PooledString& i_copy);
	inline bool operator==(const PooledString& i_other) const;
	inline bool operator==(const char* i_string) const;

private:
	const char* string_;

}; // class PooledString

} // namespace data
} // namespace engine

#include "PooledString-inl.h"

#endif // POOLED_STRING_H_
