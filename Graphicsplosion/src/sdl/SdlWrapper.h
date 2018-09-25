#pragma once

// SDL wrapper to manage globalevents, resources, initialisation etc
class SdlWrapper {
public:
	SdlWrapper();
	~SdlWrapper();

public:
	void UpdateEvents();

	bool HasReceivedQuit() {
		return hasReceivedQuit;
	}

private:
	static int numSdlUsers;

	bool hasReceivedQuit;
};

extern SdlWrapper sdl;