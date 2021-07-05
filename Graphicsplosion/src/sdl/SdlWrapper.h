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

	// Returns the relative mouse motion, in pixels, since the last UpdateEvents call
	const Vec2I& GetMouseMotion() const;

private:
	// Whether the program is being quit
	bool hasReceivedQuit;

private:
	// The mouse motion occurred since the last update
	Vec2I mouseMotion;

	// The default joystick
	SDL_Joystick* defaultJoystick;

private:
	static int numSdlUsers;
};

extern SdlWrapper sdl;