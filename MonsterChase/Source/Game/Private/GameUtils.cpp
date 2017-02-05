#include "Game\GameUtils.h"

// library includes
#include <stdio.h>

// engine includes
#include "Assert\Assert.h"
#include "GLib.h"
#include "Logger\Logger.h"
#include "Util\FileUtils.h"

// game includes
#include "Game\GameData.h"

namespace monsterchase {

GLib::Sprites::Sprite* GameUtils::CreateSprite(const char* i_filename)
{
	ASSERT(i_filename);

	size_t texture_filesize = 0;

	// Load the source file (texture data)
	uint8_t* texture_file = engine::util::FileUtils::Get()->ReadFile(i_filename, texture_filesize);
	ASSERT(texture_file);

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

}