#ifndef TIMER_UTIL_H_
#define TIMER_UTIL_H_

namespace engine {

class TimerUtil
{
private:
	TimerUtil();
	~TimerUtil();

	TimerUtil(const TimerUtil& copy);
	TimerUtil operator=(const TimerUtil& copy);

public:
	static float GetLastFrameTime_ms();
	static float CalculateLastFrameTime_ms();

	static const float					DESIRED_FPS;
	static const float					DESIRED_FRAMETIME_MS;
	static const float					MAX_FRAMETIME_MS;

private:
	// TODO: These functions may only be required for win32
	static double GetCounter();
	static double GetFrequency();

	static double							pc_frequency_;
	static double							last_frame_start_tick_;
	static float						last_frame_time_ms_;

}; // class TimerUtil

} // namespace engine

#endif // TIMER_UTIL_H_
