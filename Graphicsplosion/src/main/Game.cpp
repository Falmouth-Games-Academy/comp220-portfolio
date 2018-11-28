#include <stdafx.h>

#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include "main/Game.h"
#include "main/Time.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <iostream>

#include "helpers/types.h"

void Game::Run() {
	// Startup the game
	Init();
	
	// Set the initial time (avoid a jumpy first frame!)
	_frameTime = Time::GetTime();

	// Run the game main loop
	while (!sdl.HasReceivedQuit()) {
		// Update SDL events
		sdl.UpdateEvents();

		// Update delta time and timing
		_deltaTime = (float)(Time::GetTime() - _frameTime);
		_frameTime = Time::GetTime();

		// Update frame rate counter
		if ((int)_frameTime != (int)(_frameTime - _deltaTime)) {
			currentFps = numFramesThisSecond;
			numFramesThisSecond = 0;

			printf("fps: %i\n", currentFps);
		}

		++numFramesThisSecond;

		// Update input
		Input::Update();

		// Update the game
		Update();

		// Render the game!
		Render();

		// Check for fullscreen and escape
		if (Input::IsKeyPressed(SDL_SCANCODE_F)) {
			window.SetFullscreen(!window.IsFullscreen());
		}

		// Check for mouse unlock
		if (Input::IsKeyPressed(SDL_SCANCODE_M)) {
			window.SetMouseLock(!window.IsMouseLocked());
		}
	}

	Shutdown();
}