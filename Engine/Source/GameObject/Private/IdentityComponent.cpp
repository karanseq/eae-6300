#include "GameObject\IdentityComponent.h"

// library includes
#include <string.h>
#include <algorithm>

namespace engine {
namespace gameobject {

	IdentityComponent::IdentityComponent(uint32_t id, uint32_t tag, const char* name) : id_(id),
		tag_(tag),
		name_(_strdup(name))
	{}

	IdentityComponent::IdentityComponent(const IdentityComponent& copy)
	{
		id_ = copy.id_;
		tag_ = copy.tag_;

		SAFE_FREE(name_);
		name_ = _strdup(copy.name_);
	}

	IdentityComponent::IdentityComponent(IdentityComponent&& copy)
	{
		id_ = copy.id_;
		tag_ = copy.tag_;
		std::swap(name_, copy.name_);
	}

	IdentityComponent::~IdentityComponent()
	{
		SAFE_FREE(name_);
	}

} // namespace gameobject
} // namespace engine