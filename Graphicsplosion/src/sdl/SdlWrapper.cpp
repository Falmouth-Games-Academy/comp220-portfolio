#include "stdafx.h"
#include "SDL.h"
#include "SdlWrapper.h"
#include "main/Input.h"
#include "helpers/math.h"
#include <cassert>

// Global SDL wrapper
SdlWrapper sdl;

int SdlWrapper::numSdlUsers = 0;

SdlWrapper::SdlWrapper() : hasReceivedQuit(false) {
	/*
	SDL_GameController* SDL_GameControllerOpen(int joystick_index)
	Sint16 SDL_GameControllerGetAxis(SDL_GameController*    gamecontroller,
                                 SDL_GameControllerAxis axis)
	SDL_CONTROLLER_AXIS_LEFTX
	*/

	// Increment the reference counter
	++numSdlUsers;

	// This should be a singleton, but I refuse to use a static 'GetInstance', so this class should be used as an object with only one public instance
	assert(numSdlUsers == 1);

	// Init and setup SDL
	SDL_Init(SDL_INIT_EVERYTHING);
}

SdlWrapper::~SdlWrapper() {
	SDL_Quit();
}

void SdlWrapper::UpdateEvents() {
	SDL_Event event;

	// Reset some stuff
	mouseMotion = Vec2I(0, 0);

	// Capture SDL events
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			hasReceivedQuit = true;
		}

		if (event.type == SDL_KEYDOWN) {
			// Inform the Input class of the key event
			Input::SimulateKeyDown(event.key.keysym.scancode);
		}

		if (event.type == SDL_KEYUP) {
			Input::SimulateKeyUp(event.key.keysym.scancode);
		}

		if (event.type == SDL_MOUSEMOTION) {
			mouseMotion += Vec2I(event.motion.xrel, event.motion.yrel);
		}

		if (event.type == SDL_JOYAXISMOTION) {
			Input::SimulateJoyAxis(event.jaxis.axis, event.jaxis.value);
		}
	}
}

SDL_Scancode SdlWrapper::KeyNameToCode(const char* keyName) {
	return SDL_GetScancodeFromName(keyName);
}

const Vec2I& SdlWrapper::GetMouseMotion() const {
	return mouseMotion;
}