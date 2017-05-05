#include "Renderer\Renderer.h"

// external includes
#include "GLib.h"

// engine includes
#include "Common\HelperMacros.h"
#include "Data\PooledString.h"
#include "Util\FileUtils.h"
#include "Util\Profiler.h"

namespace engine {
namespace render {

// static member initialization
Renderer* Renderer::instance_ = nullptr;

Renderer::Renderer() : num_renderables_(0)
{}

Renderer::~Renderer()
{
	renderables_.clear();
	num_renderables_ = 0;
}

Renderer* Renderer::Create()
{
	if (!Renderer::instance_)
	{
		Renderer::instance_ = new Renderer();
	}
	return Renderer::instance_;
}

void Renderer::Destroy()
{
	SAFE_DELETE(Renderer::instance_);
}

void Renderer::Run(float i_dt)
{
    PROFILE_UNSCOPED("RendererRun");

    // Tell GLib that we want to start rendering
    GLib::BeginRendering();
    // Tell GLib that we want to render some sprites
    GLib::Sprites::BeginRendering();

    std::lock_guard<std::mutex> lock(renderables_mutex_);

    for (size_t i = 0; i < num_renderables_; ++i)
    {
        PROFILE_SCOPE_BEGIN("RenderableCall")
        renderables_[i]->Render(i_dt);
        PROFILE_SCOPE_END
    }

    // Tell GLib we're done rendering sprites
    GLib::Sprites::EndRendering();
    // Tell GLib we're done rendering
    GLib::EndRendering();
}

GLib::Sprites::Sprite* Renderer::CreateSprite(const engine::data::PooledString& i_texture_file_name)
{
    return Renderer::CreateSprite(i_texture_file_name, 255, 255, 255, 255);
}

GLib::Sprites::Sprite* Renderer::CreateSprite(const engine::data::PooledString& i_texture_file_name, uint8_t i_r, uint8_t i_g, uint8_t i_b, uint8_t i_a)
{
    // validate input
    ASSERT(i_texture_file_name.GetLength() > 0);

    // Load the source file (texture data)
    engine::util::FileUtils::FileData texture_file_data = engine::util::FileUtils::Get()->ReadFile(i_texture_file_name, true);
    ASSERT(texture_file_data.file_contents);

    // Ask GLib to create a texture out of the data (assuming it was loaded successfully)
    GLib::Texture * texture = texture_file_data.file_contents ? GLib::CreateTexture(texture_file_data.file_contents, texture_file_data.file_size) : nullptr;

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
    GLib::Sprites::SpriteEdges      Edges = { -float(width / 2.0f), float(height / 2.0f), float(width / 2.0f), -float(height / 2.0f) };
    GLib::Sprites::SpriteUVs        UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
    GLib::RGBA                      Color = { i_r, i_g, i_b, i_a };

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

} // namespace render
} // namespace engine
