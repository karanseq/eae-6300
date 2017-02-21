#include "Util\FileUtils.h"

// engine includes
#include "Assert\Assert.h"
#include "Common\HelperMacros.h"
#include "Data\HashedString.h"
#include "Logger\Logger.h"

namespace engine {
namespace util {

// static member initialization
FileUtils* FileUtils::instance_ = nullptr;

FileUtils::FileUtils()
{}

FileUtils::~FileUtils()
{
	ClearFileCache();
}

FileUtils* FileUtils::Create()
{
	if (FileUtils::instance_ == nullptr)
	{
		FileUtils::instance_ = new FileUtils();
	}
	return FileUtils::instance_;
}

void FileUtils::Destroy()
{
	SAFE_DELETE(FileUtils::instance_);
}

uint8_t* FileUtils::ReadFile(const engine::data::PooledString& i_file_name, bool i_cache_file)
{
	size_t file_size = 0;
	return ReadFile(i_file_name, file_size, i_cache_file);
}

uint8_t* FileUtils::ReadFile(const engine::data::PooledString& i_file_name, size_t& o_file_size, bool i_cache_file)
{
	// validate inputs
	ASSERT(i_file_name);
	ASSERT(strlen(i_file_name.GetString()) > 0);

	// get a hash for the file name
	unsigned int hash = engine::data::HashedString::Hash(i_file_name);

	// check if the file is in the cache
	if (IsFileCached(hash))
	{
		o_file_size = file_cache_[hash].file_size;
		return file_cache_[hash].file_contents;
	}

	// read the file
	FILE * file = nullptr;

	errno_t fopen_error = fopen_s(&file, i_file_name.GetString(), "rb");
	if (fopen_error != 0)
	{
		LOG_ERROR("Could not open %s...error code:%d", i_file_name, fopen_error);
		return nullptr;
	}

	ASSERT(file != nullptr);

	int file_IOError = fseek(file, 0, SEEK_END);
	ASSERT(file_IOError == 0);

	long file_size = ftell(file);
	ASSERT(file_size >= 0);

	file_IOError = fseek(file, 0, SEEK_SET);
	ASSERT(file_IOError == 0);

	uint8_t * buffer = new uint8_t[file_size];
	ASSERT(buffer);

	size_t file_read = fread(buffer, 1, file_size, file);
	ASSERT(file_read == file_size);

	fclose(file);
	
	// prepare outputs
	o_file_size = file_size;

	// add the file to the cache
	if (i_cache_file)
	{
		file_cache_.insert(std::pair<unsigned int, FileData>(hash, { i_file_name, buffer, o_file_size }));
		LOG("FileUtils added '%s' to the cache", i_file_name);
	}

	return buffer;
}

bool FileUtils::WriteFile(const engine::data::PooledString& i_file_name, const char* i_file_contents) const
{
	return false;
}

void FileUtils::ClearFileCache()
{
	for (auto i : file_cache_)
	{
		delete i.second.file_contents;
	}
	file_cache_.clear();
	LOG("FileUtils file cache cleared");
}

} // namespace util
} // namespace engine
