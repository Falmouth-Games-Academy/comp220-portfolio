#include "stdafx.h"
#include "Time.h"

#include <Windows.h>

bool Time::hasInitialised = false;

unsigned __int64 Time::timeAtStartup;
unsigned __int64 Time::performanceFrequency;

double Time::GetTime() {
	if (!hasInitialised) {
		LARGE_INTEGER performanceCount;

		// Get the processor frequency
		QueryPerformanceFrequency((LARGE_INTEGER*)&performanceFrequency);

		// Set initial time
		QueryPerformanceCounter(&performanceCount);
		*((LARGE_INTEGER*)&timeAtStartup) = performanceCount;
		hasInitialised = true;
	}

	// Get the current processor time
	unsigned __int64 currentTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	// Return the time in seconds
	return ((double)(currentTime - timeAtStartup) / (double)performanceFrequency);
}
