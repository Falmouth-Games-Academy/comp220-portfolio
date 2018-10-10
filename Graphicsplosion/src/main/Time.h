#pragma once
class Time {
public:
	// Returns the time since startup
	static float GetTime();

private:
	static unsigned __int64 timeAtStartup;

	static unsigned __int64 performanceFrequency;

	static bool hasInitialised;
};