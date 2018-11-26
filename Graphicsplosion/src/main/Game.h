#pragma once
#include "actors/Player.h"
#include "render/Renderer.h"
#include "sdl/Window.h"

#include <vector>

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

public:
	// Spawns an actor
	template <typename ActorType>
	ActorType* SpawnActor() {
		ActorType* newActor = new ActorType();

		actors.push_back(newActor);

		return newActor;
	}

	void DestroyActor(Actor* actor) {
		// Find and remove the actor in the actor list
		for (int i = 0; i < actors.size(); i++) {
			if (actors[i] == actor) {
				actors.erase(actors.begin() + i);
				return;
			}
		}
	}

protected:
	// Main components
	Renderer render;
	Window window;

	// Objects
	std::vector<Actor*> actors;

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