#include "Monster.h"

inline int Monster::GetTimeToLive() const 
{
	return time_to_live_;
}

inline void Monster::SetTimeToLive(uint8_t time_to_live)
{
	time_to_live_ = time_to_live;
}