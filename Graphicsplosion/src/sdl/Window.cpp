#include "stdafx.h"
#include "SDL.h"
#include "Window.h"

#include "glew.h"

void Window::Init(const char* windowTitle, const Vec2I& windowSize) {
	// Create the window
	sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
}

void* Window::CreateGlContext() {
	if (sdlGlContext || !sdlWindow) {
		// Don't create it if it already exists
		return nullptr;
	}

	// Create OpenGL context
	return (sdlGlContext = SDL_GL_CreateContext(sdlWindow));
}

void* Window::GetGlContext() {
	return sdlGlContext;
}

Vec2I Window::GetSize() const {
	// Return the window size suppiled by SDL_GetWindowSize
	Vec2I size;
	SDL_GetWindowSize(sdlWindow, &size.x, &size.y);
	return size;
}

void Window::SetFullscreen(bool isFullscreen) {
	// Setup the fullscreen display mode
	if (isFullscreen) {
		SDL_DisplayMode displayMode;

		// Use the current display mode before fullscreening
		SDL_GetCurrentDisplayMode(0, &displayMode);
		SDL_SetWindowDisplayMode(sdlWindow, &displayMode);
	}

	// Toggle fullscreen
	SDL_SetWindowFullscreen(sdlWindow, isFullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	SDL_ShowCursor(0);
}

Window::~Window() {
	if (sdlWindow) {
		// Cleanup the resources
		if (sdlGlContext) {
			SDL_GL_DeleteContext(sdlGlContext);
		}

		SDL_DestroyWindow(sdlWindow);
	}
}