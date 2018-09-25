#include "stdafx.h"
#include "SDL.h"
#include "Window.h"

Window::Window(const char* windowTitle, const Vec2I& windowSize) {
	sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
}

Window::~Window() {
	if (sdlWindow) {
		SDL_DestroyWindow(sdlWindow);
	}
}