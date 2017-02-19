#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

// library includes
#include <functional>
#include <unordered_map>

// forward declarations
namespace engine {
namespace data {
	class PooledString;
}
}

namespace engine {
namespace util {

class FileUtils
{
	struct FileData {
		size_t			file_size;
		uint8_t*		file_contents;
	}; // struct FileData

private:
	FileUtils();
	~FileUtils();
	static FileUtils* instance_;

	// disable copy constructor & copy assignment operator
	FileUtils(const FileUtils& i_copy) = delete;
	FileUtils& operator=(const FileUtils& i_copy) = delete;

public:
	static FileUtils* Create();
	static void Destroy();
	static inline FileUtils* Get();

	uint8_t* ReadFile(const engine::data::PooledString& i_file_name, bool i_cache_file = true);
	uint8_t* ReadFile(const engine::data::PooledString& i_file_name, size_t& o_file_size, bool i_cache_file = true);
	
	bool WriteFile(const char* i_file_name, const char* i_file_contents) const;
	void ClearFileCache();

	inline bool IsFileCached(const char* i_file_name) const;
	inline bool IsFileCached(unsigned int i_hash) const;

private:
	std::unordered_map<unsigned int, FileData>				file_cache_;
	
}; // class FileUtils

} // namespace util
} // namespace engine

#include "FileUtils-inl.h"

#endif // FILE_UTILS_H_
