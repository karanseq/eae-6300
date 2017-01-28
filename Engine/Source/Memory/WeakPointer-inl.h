#include "WeakPointer.h"

// library includes
#include <algorithm>

// engine includes
#include "Common\HelperMacros.h"

namespace engine {
namespace memory {

template<class T>
inline WeakPointer<T>& WeakPointer<T>::operator=(const WeakPointer& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		// release previously observed object
		Release();

		// copy data
		object_ = i_copy.object_;
		ref_counter_ = i_copy.ref_counter_;

		// acquire newly observed object
		Acquire();
	}
	return *this;
}

template<class T>
inline WeakPointer<T>& WeakPointer<T>::operator=(WeakPointer&& i_copy)
{
	// check for self assignment
	if (this != &i_copy)
	{
		// swap data
		std::swap(object_, i_copy.object_);
		std::swap(ref_counter_, i_copy.ref_counter_);
	}
	return *this;
}

template<class T>
inline WeakPointer<T>& WeakPointer<T>::operator=(const StrongPointer<T>& i_strong_pointer)
{
	// release previously observed object
	Release();

	// copy data
	object_ = i_strong_pointer.object_;
	ref_counter_ = i_strong_pointer.ref_counter_;

	// acquire newly observed object
	Acquire();

	return *this;
}

#ifdef BUILD_DEBUG
template<class T>
inline long WeakPointer<T>::GetStrongCount() const
{
	return ref_counter_ ? ref_counter_->strong_count : 0;
}

template<class T>
inline long WeakPointer<T>::GetWeakCount() const
{
	return ref_counter_ ? ref_counter_->weak_count : 0;
}
#endif

template<class T>
inline StrongPointer<T> WeakPointer<T>::Lock() const
{
	return StrongPointer<T>(*this);
}

template<class T>
inline void WeakPointer<T>::Acquire()
{
	if (ref_counter_)
	{
		++(ref_counter_->weak_count);
	}
}

template<class T>
inline void WeakPointer<T>::Release()
{
	if (ref_counter_)
	{
		if (--(ref_counter_->weak_count) <= 0 && ref_counter_->strong_count <= 0)
		{
			SAFE_DELETE(ref_counter_);
		}
		return;
	}
}

} // namespace memory
} // namespace engine