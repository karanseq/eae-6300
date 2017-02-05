#include "Renderer.h"

// libarary includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Logger\Logger.h"

namespace engine {
namespace render {

inline Renderer* Renderer::Get()
{
	return Renderer::instance_;
}

inline void Renderer::AddRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object)
{
	// validate input
	ASSERT(i_renderable_object);

	// check if this object already exists
	if (std::find(renderables_.begin(), renderables_.end(), i_renderable_object) != renderables_.end())
	{
		LOG_ERROR("Renderer is already tracking this renderable!");
		return;
	}

	renderables_.push_back(i_renderable_object);
	++num_renderables_;
}

inline void Renderer::RemoveRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object)
{
	// validate input
	ASSERT(i_renderable_object);
	// can't remove renderable if there are none
	ASSERT(num_renderables_ > 0);

	// check if this object exists
	auto it = std::find(renderables_.begin(), renderables_.end(), i_renderable_object);
	if (it == renderables_.end())
	{
		LOG_ERROR("Renderer could not find this renderable!");
		return;
	}

	renderables_.erase(it);
	--num_renderables_;
}

} // namespace render
} // namespace engine
