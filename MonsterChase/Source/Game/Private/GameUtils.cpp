#include "Game\GameUtils.h"

// library includes
#include <stdio.h>

// engine includes
#include "Assert\Assert.h"
#include "GLib.h"

// game includes
#include "Game\GameData.h"

namespace monsterchase {

GLib::Sprites::Sprite* GameUtils::CreateSprite(const char* i_filename)
{
	ASSERT(i_filename);

	size_t texture_filesize = 0;

	// Load the source file (texture data)
	void * texture_file = LoadFile(i_filename, true, texture_filesize);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture * texture = texture_file ? GLib::CreateTexture(texture_file, texture_filesize) : nullptr;

	if (texture == nullptr)
		return nullptr;

	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(texture, width, height, depth);
	ASSERT(result == true);
	ASSERT((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprites::Sprite * sprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
	if (sprite == nullptr)
	{
		GLib::Release(texture);
		return nullptr;
	}

	// Bind the texture to sprite
	GLib::Sprites::SetTexture(*sprite, *texture);

	return sprite;
}

uint8_t* GameUtils::LoadFile(const char* i_filename, bool i_cache_file, size_t& o_filesize)
{
	ASSERT(i_filename);
	const std::string filename_str(i_filename);

	// first check if the file is in the cache
	GameData* game_data = GameData::GetInstance();
	if (game_data->file_cache_.find(filename_str) != game_data->file_cache_.end())
	{
		const FileData& file_data = game_data->file_cache_[filename_str];
		o_filesize = file_data.file_size;
		return file_data.file_content;
	}

	FILE * file = nullptr;

	errno_t fopen_error = fopen_s(&file, i_filename, "rb");
	if (fopen_error != 0)
		return nullptr;

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

	o_filesize = file_size;

	if (i_cache_file)
	{
		// add them to the cache
		game_data->AddFileToCache(filename_str, o_filesize, buffer);
	}

	return buffer;
}

uint8_t* GameUtils::LoadFile(const char* i_filename, bool i_cache_file)
{
	size_t file_size = 0;
	return GameUtils::LoadFile(i_filename, i_cache_file, file_size);
}

}