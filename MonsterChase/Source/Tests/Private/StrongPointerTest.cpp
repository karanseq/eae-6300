// engine includes
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"
#include "Math\Vec3D.h"
#include "Memory\StrongPointer.h"
#include "Memory\WeakPointer.h"

void TestStrongPointer()
{
	LOG("-------------------- Running StrongPointer Test --------------------");
	
	// block that tests the strong pointer constructors and assignment operators
	{
		// standard constructor...strong pointer to nullptr
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr1;
		LOG("StrongPointer standard constructor object = nullptr OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr1.GetStrongCount(), strong_ptr1.GetWeakCount());
#endif

		// standard constructor...strong pointer to a game object
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr2(new engine::gameobject::GameObject());
		LOG("StrongPointer standard constructor object = game object OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr2.GetStrongCount(), strong_ptr2.GetWeakCount());
#endif

		// copy constructor
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr3(strong_ptr2);
		LOG("StrongPointer copy constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr3.GetStrongCount(), strong_ptr3.GetWeakCount());
#endif

		// move constructor
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr4(std::move(strong_ptr3));
		LOG("StrongPointer move constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr4.GetStrongCount(), strong_ptr4.GetWeakCount());
#endif

		// copy assignment
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr5;
		strong_ptr5 = strong_ptr4;
		LOG("StrongPointer copy assignment operator OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr5.GetStrongCount(), strong_ptr5.GetWeakCount());
#endif

		// move assignment
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr6;
		strong_ptr6 = std::move(strong_ptr5);
		LOG("StrongPointer move assignment operator OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr6.GetStrongCount(), strong_ptr6.GetWeakCount());
#endif

		// weak pointer (non-nullptr) constructor
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr1(strong_ptr6);
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr7(weak_ptr1);
		LOG("StrongPointer weak pointer to non-nullptr constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr7.GetStrongCount(), strong_ptr7.GetWeakCount());
#endif

		// weak pointer (nullptr) constructor
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr2;
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr8(weak_ptr2);
		LOG("StrongPointer weak pointer  to nullptr constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr8.GetStrongCount(), strong_ptr8.GetWeakCount());
#endif

		// assignment to nullptr
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr9(new engine::gameobject::GameObject());
		strong_ptr9 = nullptr;
		LOG("StrongPointer assignment to nullptr OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr9.GetStrongCount(), strong_ptr9.GetWeakCount());
#endif

		// assignment to a pointer
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr10(nullptr);
		strong_ptr10 = new engine::gameobject::GameObject();
		LOG("StrongPointer assignment to game object OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr10.GetStrongCount(), strong_ptr10.GetWeakCount());
#endif
	}

	LOG("------------------------------");

	// block that tests remaining strong pointer operators
	{
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr1(new engine::gameobject::GameObject());
		
		// member access operator
		strong_ptr1->SetPosition(strong_ptr1->GetPosition() + engine::math::Vec3D(10.0f, 10.0f, 10.0f));

		// dereference operator
		const engine::math::Vec3D& vec = (*strong_ptr1).GetPosition();
		LOG("Member access operator & dereference operator test vec:%f, %f, %f", vec.x(), vec.y(), vec.z());

		// bool operator
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr2;
		LOG("Strong pointer 2 bool operator:%s", (strong_ptr2) ? "true" : "false");

		// equality check with nullptr
		LOG("Strong pointer 2 == nullptr:%s", (strong_ptr2 == nullptr) ? "true" : "false");

		// inequality check with nullptr
		LOG("Strong pointer 2 != nullptr:%s", (strong_ptr2 != nullptr) ? "true" : "false");

		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr3(new engine::gameobject::GameObject());
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr4 = strong_ptr1;

		// equality check with another pointer
		LOG("Strong pointer 3 == Strong pointer 4:%s", (strong_ptr3 == strong_ptr4) ? "true" : "false");

		// inequality check with another pointer
		LOG("Strong pointer 4 == Strong pointer 1:%s", (strong_ptr4 == strong_ptr1) ? "true" : "false");
	}
	
	LOG("------------------------------");

	// block that tests weak pointer constructors & operators
	{
		// standard constructor
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr0;
		LOG("WeakPointer standard constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr0.GetStrongCount(), weak_ptr0.GetWeakCount());
#endif

		// strong pointer constructor
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr0(new engine::gameobject::GameObject());
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr1 = strong_ptr0;
		LOG("WeakPointer strong pointer constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr1.GetStrongCount(), weak_ptr1.GetWeakCount());
#endif

		// copy constructor
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr2 = weak_ptr1;
		LOG("WeakPointer copy constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr2.GetStrongCount(), weak_ptr2.GetWeakCount());
#endif

		// move constructor
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr3(std::move(weak_ptr1));
		LOG("WeakPointer move constructor OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr3.GetStrongCount(), weak_ptr3.GetWeakCount());
#endif

		// copy assignment
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr4;
		weak_ptr4 = weak_ptr3;
		LOG("WeakPointer copy assignment OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr4.GetStrongCount(), weak_ptr4.GetWeakCount());
#endif

		// move assignment
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr5;
		weak_ptr5 = std::move(weak_ptr4);
		LOG("WeakPointer move assignment OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr5.GetStrongCount(), weak_ptr5.GetWeakCount());
#endif

		// strong pointer assignment
		engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr6;
		weak_ptr6 = strong_ptr0;
		LOG("WeakPointer strong pointer assignment OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr6.GetStrongCount(), weak_ptr6.GetWeakCount());
#endif

		// weak pointer lock function
		engine::memory::StrongPointer<engine::gameobject::GameObject> strong_ptr1 = weak_ptr6.Lock();
		LOG("WeakPointer Lock function OK!");
#ifdef BUILD_DEBUG
		LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr1.GetStrongCount(), strong_ptr1.GetWeakCount());
#endif
	}
	
	LOG("-------------------- Finished StrongPointer Test --------------------");
}