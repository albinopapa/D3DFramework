#include "Timer.h"


Timer::Timer()
{
	Start();
}

Timer::~Timer()
{}

using std::chrono::time_point;
using precise_clock = std::chrono::high_resolution_clock;

void Timer::Start()
{
	start = precise_clock::now();
	watchStopped = false;
}

void Timer::Stop()
{
	stop = precise_clock::now();
	watchStopped = true;
}

float Timer::Reset()
{	
#ifdef _DEBUG
	return .016f;
#else
	return AutoReset( *this ).GetElapsed();
#endif
}

float Timer::GetElapsed()
{
	return (watchStopped)? 
		std::chrono::duration<float>( stop - start ).count():
		std::chrono::duration<float>( precise_clock::now() - start ).count();
}




