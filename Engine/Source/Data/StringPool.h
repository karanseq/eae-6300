#ifndef STRING_POOL_H_
#define STRING_POOL_H_

// library includes
#include <stdint.h>

namespace engine {
namespace data {

// TODO: Add support for multiple pools
class StringPool
{
private:
	explicit StringPool(uint8_t* i_pool, size_t i_pool_size);
	~StringPool();
	static StringPool* instance_;

	// disable copy constructor & copy assignment operator
	StringPool(const StringPool& i_copy) = delete;
	StringPool& operator=(const StringPool&) = delete;

public:
	static StringPool* Create(size_t i_bytes = DEFAULT_POOL_SIZE);
	static void Destroy();
	static inline StringPool* Get();

	const char* Add(const char* i_string);
	const char* Find(const char* i_string);

private:
	inline bool Contains(const char* i_string) const;

public:
	// constants
	static const size_t								DEFAULT_POOL_SIZE;

private:
	uint8_t*										pool_;
	size_t											pool_size_;
};

} // namespace data
} // namespace engine

#include "StringPool-inl.h"

#endif // STRING_POOL_H_