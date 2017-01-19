#include "Timer\TimerUtil.h"

// library includes
#include <Windows.h>

namespace engine {

float TimerUtil::CalculateLastFrameTime_ms()
{
	// grab the CPU tick counter
	double current_tick = GetCounter();

	if (last_frame_start_tick_)
	{
		// how many ticks have passed since the last time this function was called?
		double elapsed_ticks = current_tick - last_frame_start_tick_;

		// calculate time in milliseconds since the last time this function was called?
		last_frame_time_ms_ = float(elapsed_ticks / GetFrequency());
	}
	else
	{
		last_frame_time_ms_ = 16.66667f;
	}
	// save the current frame's tick
	last_frame_start_tick_ = current_tick;

	return last_frame_time_ms_;
}

double TimerUtil::GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart);
}

double TimerUtil::GetFrequency()
{
	if (pc_frequency_ <= 0)
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		pc_frequency_ = double(li.QuadPart);
	}
	return pc_frequency_;
}

} // namespace engine
