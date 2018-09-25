#pragma once
#include "helpers/vector.h"

// Window class using SDL windows
class Window {
public:
	// Constructors and destructors
	Window(const char* windowTitle, const Vec2I& windowSize);
	~Window();

public:
	// Returns the error state where 0 = no errors in creation
	int GetError() {
		return (int)(sdlWindow != nullptr);
	}

private:
	struct SDL_Window* sdlWindow;
};