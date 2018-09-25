#include "stdafx.h"
#include "SDL.h"
#include "SdlWrapper.h"
#include <cassert>

// Global SDL wrapper
SdlWrapper sdl;

SdlWrapper::SdlWrapper() : hasReceivedQuit(false) {
	// Increment the reference counter
	++numSdlUsers;

	// This should be a singleton, but I refuse to use a static 'GetInstance', so this class should be used as an object with only one public instance
	assert(numSdlUsers == 1);

	SDL_Init(SDL_INIT_EVERYTHING);
}

SdlWrapper::~SdlWrapper() {
	SDL_Quit();
}

void SdlWrapper::UpdateEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			hasReceivedQuit = true;
		}
	}
}