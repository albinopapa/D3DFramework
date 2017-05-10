#pragma once

#include <chrono>

class Timer
{
	class AutoReset
	{
	public:
		AutoReset( Timer &Parent ) :parent( Parent ) { parent.Stop(); }
		float GetElapsed()const{ return parent.GetElapsed(); }
		~AutoReset() { parent.Start(); }
	private:
		Timer &parent;
	};
public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	float Reset();
private:
	float GetElapsed();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
	bool watchStopped = true;
};

