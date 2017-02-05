#ifndef RENDERER_H_
#define RENDERER_H_

// library includes
#include <vector>

// engine includes
#include "Memory\SharedPointer.h"
#include "RenderableObject.h"

namespace engine {
namespace render {

class Renderer
{
private:
	Renderer();
	~Renderer();
	static Renderer* instance_;

	// disable copy constructor & copy assignment operator
	Renderer(const Renderer& i_copy) = delete;
	Renderer& operator=(const Renderer& i_copy) = delete;

public:
	static Renderer* Create();
	static void Destroy();
	static inline Renderer* Get();

	void Run(float i_dt);

	// add/remove renderer objects
	inline void AddRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object);
	inline void RemoveRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object);

private:
	size_t																			num_renderables_;
	std::vector<engine::memory::SharedPointer<RenderableObject>>					renderables_;

}; // class Renderer

} // namespace render
} // namespace engine

#include "Renderer-inl.h"

#endif // RENDERER_H_