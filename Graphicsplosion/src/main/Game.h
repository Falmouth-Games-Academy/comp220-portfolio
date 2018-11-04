#pragma once
#include "actors/Player.h"
#include "render/Renderer.h"
#include "sdl/Window.h"

// The entire game. Call Run() to run it!
class Game {
public:
	Game() : deltaTime(_deltaTime), frameTime(_frameTime) {

	}

	// Runs the game
	void Run();

private:
	// Called on game initialisation
	virtual void Init() = 0;
	
	// Called on game shutdown
	virtual void Shutdown() = 0;

private:
	// Called during each frame
	virtual void Update() = 0;

	// Called each frame after Update() when the renderer is ready
	virtual void Render() = 0;

protected:
	// Main components
	Renderer render;
	Window window;

	// Timing
	float _deltaTime;  // time since the last frame
	double _frameTime; // time since the game began

	// FPS
	unsigned int currentFps;
	unsigned int numFramesThisSecond;

public:
	// Public read-only variables
	// Time since the last update
	const float& deltaTime;

	// Time at the beginning of the frame, in seconds
	const double& frameTime;
};