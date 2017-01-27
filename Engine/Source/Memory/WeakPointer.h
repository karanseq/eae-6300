#ifndef WEAK_POINTER_H_
#define WEAK_POINTER_H_

// engine includes
#include "Memory\StrongPointer.h"

namespace engine {
namespace memory {

template<class T>
class WeakPointer
{
public:
	WeakPointer() : object_(nullptr),
		ref_counter_(nullptr)
	{}
	~WeakPointer()
	{
		Release();
	}

	WeakPointer(const WeakPointer& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		Acquire();
	}

	WeakPointer(WeakPointer&& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		i_copy.object_ = nullptr;
		i_copy.ref_counter_ = nullptr;
	}

	WeakPointer(const StrongPointer<T>& i_strong_pointer) : object_(i_strong_pointer.object_),
		ref_counter_(i_strong_pointer.ref_counter_)
	{
		Acquire();
	}

	inline WeakPointer& operator=(const WeakPointer& i_copy);
	inline WeakPointer& operator=(WeakPointer&& i_copy);
	inline WeakPointer& operator=(const StrongPointer<T>& i_strong_pointer);

#ifdef BUILD_DEBUG
	inline long GetStrongCount() const;
	inline long GetWeakCount() const;
#endif

	inline StrongPointer<T> Lock() const;

private:
	inline void Acquire();
	inline void Release();

private:
	T*								object_;
	RefCounter*						ref_counter_;

	template<class T>
	friend class StrongPointer;

}; // class WeakPointer

} // namespace memory
} // namespace engine

#include "WeakPointer-inl.h"

#endif // WEAK_POINTER_H_
