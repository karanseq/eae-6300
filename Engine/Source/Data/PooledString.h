#ifndef POOLED_STRING_H_
#define POOLED_STRING_H_

// engine includes
#include "StringPool.h"

namespace engine {
namespace data {

class PooledString
{
public:
	PooledString();
	explicit PooledString(const char* i_string);
	~PooledString();

	PooledString(const PooledString& i_copy);

	inline PooledString& operator=(const PooledString& i_copy);
	inline bool operator==(const PooledString& i_other) const;
	
	inline const char* GetString() const;

private:
	const char*									string_;

}; // class PooledString

} // namespace data
} // namespace engine

#include "PooledString-inl.h"

#endif // POOLED_STRING_H_
