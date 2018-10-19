#include <stdafx.h>

#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include "main/Game.h"
#include "main/Time.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

struct Vertex {
	float x, y, z;
	float r, g, b;
};

Game game;

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
		_deltaTime = Time::GetTime() - _frameTime;
		_frameTime = Time::GetTime();

		// Update frame rate counter
		if ((int)_frameTime != (int)(_frameTime - _deltaTime)) {
			currentFps = numFramesThisSecond;
			numFramesThisSecond = 0;

			printf("fps: %i\n", currentFps);
		}

		++numFramesThisSecond;

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

void Game::Init() {
	// Initialise window and renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	Vertex a = { 0.0f };
	//Iterate<Vertex>(&a, a.r, a.g, a.b);

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Init(render, vertexShader, fragmentShader);

	// Points for the test triangles
	static GLfloat g_vertex_buffer_data[] = {
		-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	// Create the test triangle
	vertBuf.Create(render, g_vertex_buffer_data, sizeof (g_vertex_buffer_data));
	
	// Setup the default vertex format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));

	// Spawn the player
	player.OnSpawn();
}

void Game::Shutdown() {
	// Clean up the test vertex buffer
	vertBuf.Destroy();
}

void Game::Update() {
	// Update input
	Input::Update();

	// Update the player
	player.Update(deltaTime);
}

void Game::Render() {
	// Render here
	render.BeginRender();

	// Use our test shader
	render.UseShaderProgram(defaultShaderProgram);

	// Test rotate the triangle
	static float angle;
	angle += 6.28f * deltaTime;

	// Find the shader constants
	int uniMatWorld = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matWorld");
	int uniMatViewProj = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matViewProj");
	int uniTime = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "time");

	// Set up the view/proj matrix
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;

	glm::mat4 matViewProj = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());
	matViewProj = glm::perspectiveFov(70.0f, (float)window.GetSize().x, (float)window.GetSize().y, 0.1f, 100.0f) * matViewProj;

	// Set up the world matrix (just a fun rotation around angle)
	glm::mat4 matWorld = glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));

	// Upload the uniform variables
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, (GLfloat*)&matWorld);
	glUniformMatrix4fv(uniMatViewProj, 1, GL_FALSE, (GLfloat*)&matViewProj);
	glUniform1f(uniTime, Time::GetTime());

	// Enable the vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// Draw the triangle
	render.UseVertexBuffer(vertBuf);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Done!
	render.EndRender(window);
}