#include "stdafx.h"
#include "SDL.h"
#include "SdlWrapper.h"
#include "helpers/math.h"
#include <cassert>

// Global SDL wrapper
SdlWrapper sdl;

int SdlWrapper::numSdlUsers = 0;

SdlWrapper::SdlWrapper() : hasReceivedQuit(false) {
	// Increment the reference counter
	++numSdlUsers;

	// This should be a singleton, but I refuse to use a static 'GetInstance', so this class should be used as an object with only one public instance
	assert(numSdlUsers == 1);

	// Init and setup SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
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

		if (event.type == SDL_MOUSEMOTION) {
			mouseMotion = Vec2I(event.motion.xrel, event.motion.yrel);
		}
	}
}

bool SdlWrapper::CheckKeyDown(const char* scancodeName) {
	SDL_Scancode scanCode = SDL_GetScancodeFromName(scancodeName);

	if (scanCode != SDL_SCANCODE_UNKNOWN) {
		return SDL_GetKeyboardState(nullptr)[scanCode] != 0;
	} else {
		return false;
	}
}

const Vec2I& SdlWrapper::GetMouseMotion() const {
	return mouseMotion;
}