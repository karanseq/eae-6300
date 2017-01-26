#ifndef REF_COUNTER_H_
#define REF_COUNTER_H_

namespace engine {
namespace memory {

struct RefCounter
{
public:
	RefCounter(long i_strong_count = 0, long i_weak_count = 0) : strong_count_(i_strong_count),
		weak_count_(i_weak_count)
	{}
	long strong_count_;
	long weak_count_;
}; // struct RefCounter

} // namespace memory
} // namespace engine

#endif // REF_COUNTER_H_
