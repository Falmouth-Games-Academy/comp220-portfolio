#pragma once
#include "actors/Player.h"
#include "render/Renderer.h"
#include "sdl/Window.h"

// The entire game. Call Run() to run it!
class Game {
public:
	Game() : deltaTime(_deltaTime), frameTime(_frameTime) {

	}

	void Run();

private:
	void Init();
	void Shutdown();

private:
	void Update();
	void Render();

private:
	// Main components
	Player player;
	Renderer render;
	Window window;

	// Rendering components
	ShaderProgram defaultShaderProgram;

	// Timing
	float _deltaTime;
	float _frameTime;

public:
	// Public read-only variables
	// Time since the last update
	const float& const deltaTime;

	// Time at the beginning of the frame, in seconds
	const float& const frameTime;
};

// The entire game lies here
extern Game game;