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

VertexBuffer vertBuf;

/*void GenCube(float dimension, Vertex verts[36]) {
	glm::vec3 right(1.0f, 0.0f, 0.0f);
	glm::vec3 forward(0.0f, 1.0f, 0.0f);
	glm::vec3 up(0.0f, 0.0f, 1.0f);

	struct dim {
		glm::vec3 forward, right, up;
	};

	dim dimensions[] = { forward, right, up, 
						-forward, right, up, 
						 up, right, forward, 
						-up, right, forward, 
						 right, up, forward,
						-right, up, forward};
	glm::vec3 v[36];

	for (int i = 0; i < 36; i += 6) {
		glm::vec3& forward = dimensions[i / 6].forward, &right = dimensions[i / 6].right, &up = dimensions[i / 6].up;
		v[i + 0] = forward + up + right;
		v[i + 1] = forward + up - right;
		v[i + 2] = forward - up - right;

		v[i + 3] = forward + up + right;
		v[i + 4] = forward - up - right;
		v[i + 5] = forward - up + right;
	}

	for (int i = 0; i < 36; i++) {
		verts[i].x = v[i].x;
		verts[i].y = v[i].y;
		verts[i].z = v[i].z;
	}
}*/

struct VertAttribs {
	int stride;
	GLenum type;
	int offset;
	int numItems;
	VertAttribs* previous;
};

template<typename T> void GetGlVertAttribs(VertAttribs* previous, T* dudeWtf);

template<> void GetGlVertAttribs<int>(VertAttribs* previous, int* dudeWtf) {
	VertAttribs attribs = { 0, GL_FLOAT, };
	printf("Parameter is an int!\n");
}

template<> void GetGlVertAttribs<float>(VertAttribs* previous, float* dudeWtf) {
	printf("Parmeter is a float!\n");
}

template<typename VertexType, typename VarType, typename ...types> void Iterate(VertexType* vertex, VarType VertexType::*, types... arguments) {
	VertAttribs initialAttribs = { sizeof(vertex) };
	initialAttribs.previous = nullptr;

	GetGlParameter(initialAttribs, &);

	Iterate(vertex, arguments...);
}

void Iterate(Vertex* vertex) {
	printf("Done\n");
}