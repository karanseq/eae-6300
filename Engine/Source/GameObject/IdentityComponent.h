#ifndef ENGINE_IDENTITY_COMPONENT_H_
#define ENGINE_IDENTITY_COMPONENT_H_

// library includes
#include <cstdint>
#include <string.h>

// engine includes
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

namespace engine {
namespace gameobject {

	class IdentityComponent
	{
	public:
		IdentityComponent(uint32_t i_id = 0, uint32_t i_tag = 0, const char* i_name = "") : id_(i_id),
			tag_(i_tag),
			name_(_strdup(i_name))
		{}
	
		IdentityComponent(const IdentityComponent& i_copy)
		{
			id_ = i_copy.GetID();
			tag_ = i_copy.GetTag();
			
			SAFE_FREE(name_);
			name_ = _strdup(i_copy.GetName());
		}

		~IdentityComponent()
		{
			SAFE_FREE(name_);
		}

		// accessors and mutators
		inline void SetID(uint32_t i_id)								{ id_ = i_id; }
		inline uint32_t GetID() const									{ return id_; }

		inline void SetTag(uint32_t i_tag)								{ tag_ = i_tag; }
		inline uint32_t GetTag() const									{ return tag_; }

		inline void SetName(const char* i_name)							{ ASSERT(i_name); SAFE_FREE(name_); name_ = _strdup(i_name); }
		inline const char* GetName() const								{ return name_; }

		// assignment
		inline IdentityComponent& operator=(const IdentityComponent& i_ic)
		{
			// check for self assignment
			if (this == &i_ic)
			{
				return *this;
			}

			id_ = i_ic.id_;
			tag_ = i_ic.tag_;

			SAFE_FREE(name_);
			name_ = _strdup(i_ic.name_);
			
			return *this;
		}

	protected:
		uint32_t			id_;
		uint32_t			tag_;
		char*				name_;
	}; // class IdentityComponent

} // namespace gameobject
} // namespace engine

#endif // ENGINE_IDENTITY_COMPONENT_H_
