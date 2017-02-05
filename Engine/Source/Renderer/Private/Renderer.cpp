#include "Renderer\Renderer.h"

// engine includes
#include "Common\HelperMacros.h"

namespace engine {
namespace render {

// static member initialization
Renderer* Renderer::instance_ = nullptr;

Renderer::Renderer() : num_renderables_(0)
{}

Renderer::~Renderer()
{}

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
	for (size_t i = 0; i < num_renderables_; ++i)
	{
		renderables_[i]->Render(i_dt);
	}
}

} // namespace render
} // namespace engine
