#ifndef ENGINE_IDENTITY_COMPONENT_H_
#define ENGINE_IDENTITY_COMPONENT_H_

// library includes
#include <cstdint>
#include <string.h>

// engine includes
#include "Memory\AllocatorUtil.h"
#include "Assert\Assert.h"

namespace engine
{
	class IdentityComponent
	{
	public:
		IdentityComponent(uint32_t id = 0, uint32_t tag = 0, const char* name = "") : id_(id),
			tag_(tag),
			name_(_strdup(name))
		{}
	
		IdentityComponent(const IdentityComponent& copy)
		{
			id_ = copy.GetID();
			tag_ = copy.GetTag();
			SAFE_FREE(name_);
			name_ = _strdup(copy.GetName());
		}

		~IdentityComponent()
		{
			SAFE_FREE(name_);
		}

		// accessors and mutators
		inline void SetID(uint32_t id)								{ id_ = id; }
		inline uint32_t GetID() const								{ return id_; }

		inline void SetTag(uint32_t tag)							{ tag_ = tag; }
		inline uint32_t GetTag() const								{ return tag_; }

		inline void SetName(const char* name)						{ ASSERT(name); SAFE_FREE(name_); name_ = _strdup(name); }
		inline const char* GetName() const							{ return name_; }

		// assignment
		inline IdentityComponent& operator=(const IdentityComponent& ic)
		{
			// check for self assignment
			if (this == &ic)
			{
				return *this;
			}

			id_ = ic.id_;
			tag_ = ic.tag_;
			SAFE_FREE(name_);
			name_ = _strdup(ic.name_);
			return *this;
		}

	protected:
		uint32_t id_;
		uint32_t tag_;
		char* name_;
	}; // class IdentityComponent

} // namespace engine

#endif // ENGINE_IDENTITY_COMPONENT_H_
