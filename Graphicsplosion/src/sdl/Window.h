#pragma once
#include "helpers/math.h"

// Window class using SDL windows
class Window {
public:
	// Constructs but doesn't create
	Window() : sdlWindow(nullptr), sdlGlContext(nullptr), isFullscreen(false), isMouseLocked(false) {}

	// Constructs and creates the window
	Window(const char* windowTitle, const Vec2I& windowSize) {
		Create(windowTitle, windowSize);
	}

	~Window() {
		Destroy();
	}

public:
	// Creates the window if it hasn't already been created
	void Create(const char* windowTitle, const Vec2I& windowSize);

	// Destroys the window
	void Destroy();

	// Returns the error state where 0 = no errors in creation
	int GetError() const {
		return (int)(sdlWindow != nullptr);
	}

public:
	// Tries to create a GL context. If successful, returns the context. If the context already exists, nullptr is returned. Use GetGlContext in this case
	void* CreateGlContext();

public:
	// Getters
	void* GetGlContext();
	
	struct SDL_Window* GetSdlWindow() {
		return sdlWindow;
	}

	// Returns the client area of the window
	Vec2I GetSize() const;

public:
	// Whether the window is fullscreened
	void SetFullscreen(bool isFullscreen);
	bool IsFullscreen() const;

	// Whether the mouse is locked to the window
	void SetMouseLock(bool isMouseLocked);
	bool IsMouseLocked() const;

private:
	// SDL resources
	struct SDL_Window* sdlWindow;
	void* sdlGlContext;

	// Used to remember whether the screen is fullscreen
	bool isFullscreen;

	// Used to remember whether the mouse is in locked/relative mode
	bool isMouseLocked;
};