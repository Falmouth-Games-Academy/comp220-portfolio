#pragma once
#include "helpers/math.h"

// SDL wrapper to manage globalevents, resources, initialisation etc
class SdlWrapper {
public:
	SdlWrapper();
	~SdlWrapper();

public:
	// Updates the SDL event loop
	void UpdateEvents();

	// Returns whether the program has been quit by the user
	bool HasReceivedQuit() {
		return hasReceivedQuit;
	}

public:
	// Checks if the given keyboard key is pressed down
	bool CheckKeyDown(const char* scancodeName);

	const Vec2I& GetMouseMotion() const;

private:
	Vec2I mouseMotion;

	bool hasReceivedQuit;

private:
	static int numSdlUsers;
};

extern SdlWrapper sdl;