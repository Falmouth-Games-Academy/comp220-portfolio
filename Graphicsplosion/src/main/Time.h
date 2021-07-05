#pragma once
class Time {
public:
	// Returns the time since app startup, in seconds
	static double GetTime();

private:
	// Performance counter at startup, invalid until GetTime() is called for the first time
	static unsigned __int64 timeAtStartup;

	// Frequency of the performance counter, invalid until GetTime() is called for the first time
	static unsigned __int64 performanceFrequency;

	// Whether the time has been initialised (true after GetTime() is called)
	static bool hasInitialised;
};