#ifndef STRONG_POINTER_H_
#define STRONG_POINTER_H_

// engine includes
#include "Memory\RefCounter.h"

namespace engine {
namespace memory {

// forward declarations
struct RefCounter;

template<class T>
class StrongPointer
{
public:
	StrongPointer(T* i_object = nullptr) : object_(i_object),
		ref_counter_(new RefCounter(object_ == nullptr ? 0 : 1))
	{};
	~StrongPointer()
	{
		Release();
	};

	//StrongPointer(T* i_object, RefCounter* i_ref_counter);

	StrongPointer(const StrongPointer& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		Acquire();
	}

	StrongPointer(StrongPointer&& i_copy) : object_(i_copy.object_),
		ref_counter_(i_copy.ref_counter_)
	{
		i_copy.object_ = nullptr;
		i_copy.ref_counter_ = nullptr;
	}

	inline StrongPointer& operator=(const StrongPointer& i_copy);
	inline StrongPointer& operator=(StrongPointer&& i_copy);

	inline T* operator->() const;
	inline T& operator*() const;

	inline operator bool() const;

	inline bool operator==(const StrongPointer& i_other) const;
	inline bool operator!=(const StrongPointer& i_other) const;

#ifdef BUILD_DEBUG
	inline long GetStrongCount() const;
	inline long GetWeakCount() const;
#endif // BUILD_DEBUG

private:
	inline void Acquire();
	inline void Release();

private:
	T*								object_;
	RefCounter*						ref_counter_;

}; // class StrongPointer

} // namespace memory
} // namespace engine

#include "StrongPointer-inl.h"

#endif // STRONG_POINTER_H_
