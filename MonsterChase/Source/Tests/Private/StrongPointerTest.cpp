// engine includes
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"
#include "Memory\StrongPointer.h"

void TestStrongPointer()
{
	LOG("-------------------- Running StrongPointer Test --------------------");
	
	// block that creates a strong pointer then copies it
	{
		engine::memory::StrongPointer<engine::gameobject::GameObject> ptr_1(new engine::gameobject::GameObject());
#ifdef BUILD_DEBUG
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
#endif
		engine::memory::StrongPointer<engine::gameobject::GameObject> ptr_2 = ptr_1;

#ifdef BUILD_DEBUG
		LOG("After ptr_2(ptr_1)...");
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
		LOG("ptr_2 Ref count:%zu", ptr_2.GetStrongCount());
#endif
	}

	// block that creates a strong pointer then assigns itself to another
	{
		engine::memory::StrongPointer<engine::gameobject::GameObject> ptr_1(new engine::gameobject::GameObject());
#ifdef BUILD_DEBUG
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
#endif

		engine::memory::StrongPointer<engine::gameobject::GameObject> ptr_2(nullptr);
#ifdef BUILD_DEBUG
		LOG("ptr_2 Ref count:%zu", ptr_2.GetStrongCount());
#endif

		ptr_2 = ptr_1;
#ifdef BUILD_DEBUG
		LOG("After ptr_2 = ptr_1...");
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
		LOG("ptr_2 Ref count:%zu", ptr_2.GetStrongCount());
#endif

		ptr_1 = nullptr;
#ifdef BUILD_DEBUG
		LOG("After ptr_1 = nullptr...");
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
		LOG("ptr_2 Ref count:%zu", ptr_2.GetStrongCount());
#endif

		ptr_2 = std::move(ptr_1);
#ifdef BUILD_DEBUG
		LOG("After ptr_2 = std::move(ptr_1)");
		LOG("ptr_1 Ref count:%zu", ptr_1.GetStrongCount());
		LOG("ptr_2 Ref count:%zu", ptr_2.GetStrongCount());
#endif
	}
	
	LOG("-------------------- Finished StrongPointer Test --------------------");
}