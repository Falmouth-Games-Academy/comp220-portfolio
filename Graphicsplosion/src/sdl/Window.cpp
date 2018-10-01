#include "stdafx.h"
#include "SDL.h"
#include "Window.h"

#include "glew.h"

Window::Window(const char* windowTitle, const Vec2I& windowSize) {
	// Create the window
	sdlWindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	// Create OpenGL context
	sdlGlContext = SDL_GL_CreateContext(sdlWindow);

	// Initialise OpenGL attributes (may move later)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

	// Setup GLEW
	glewExperimental = GL_TRUE;

	glewInit();

	// Setup GL triangle
	GLfloat triangle_vertices[] = {
		0.0,  0.8,
		-0.8, -0.8,
		0.8, -0.8,
	};
}

void Window::BeginRender(bool doClear) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::EndRender() {
	SDL_GL_SwapWindow(sdlWindow);
}

Window::~Window() {
	if (sdlWindow) {
		SDL_GL_DeleteContext(sdlGlContext);

		SDL_DestroyWindow(sdlWindow);
	}
}