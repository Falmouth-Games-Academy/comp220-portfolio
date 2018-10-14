#include "stdafx.h"
#include "SDL.h"
#include "Window.h"

#include "glew.h"

void Window::Init(const char* windowTitle, const Vec2I& windowSize) {
	// Create the window
	sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
}

void* Window::CreateGlContext() {
	if (sdlGlContext) {
		return nullptr;
	}

	// Create OpenGL context
	return (sdlGlContext = SDL_GL_CreateContext(sdlWindow));
}

void* Window::GetGlContext() {
	return sdlGlContext;
}

Window::~Window() {
	if (sdlWindow) {
		if (sdlGlContext) {
			SDL_GL_DeleteContext(sdlGlContext);
		}

		SDL_DestroyWindow(sdlWindow);
	}
}