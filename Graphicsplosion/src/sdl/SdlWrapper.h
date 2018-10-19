#pragma once
#include "helpers/math.h"
#include <SDL.h>

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
	// Converts a key to a scancode
	SDL_Scancode KeyNameToCode(const char* keyName);

	const Vec2I& GetMouseMotion() const;

private:
	Vec2I mouseMotion;

	bool hasReceivedQuit;

private:
	static int numSdlUsers;
};

extern SdlWrapper sdl;