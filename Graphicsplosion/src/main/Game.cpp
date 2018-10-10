#include <stdafx.h>

#include "sdl/SdlWrapper.h"
#include "sdl/Window.h"
#include "render/renderer.h"
#include "main/Game.h"
#include "main/Time.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

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

		// Update delta time
		_deltaTime = Time::GetTime() - _frameTime;
		_frameTime = Time::GetTime();

		// Update the game
		Update();

		// Render the game!
		Render();
	}

	Shutdown();
}

GLuint vertexbuffer; // temp
void Game::Init() {
	// Initialise window and renderer
	window.Init("Tester", Vec2I(640, 480));
	render.Init(window);

	// Load the shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Init(render, vertexShader, fragmentShader);

	// Create a new vertex array
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static GLfloat g_vertex_buffer_data[] = {
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f,  0.0f, 0.0f,

		-5.0f, -5.0f, 0.0f,
		-5.0f, 5.0f, 0.0f,
		5.0f, 5.0f, 0.0f,

		-5.0f, -5.0f, 0.0f,
		5.0f, 5.0f, 0.0f,
		5.0f, -5.0f, 0.0f
	};

	glEnable(GL_DEPTH_TEST);

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // number of values
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// Player starts at 0.0, -1.0, 0.0
	player.SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
}

void Game::Shutdown() {
	glDeleteBuffers(1, &vertexbuffer);
}

void Game::Update() {
	// Update input
	Input::Update();

	// Update the player
	player.Update(deltaTime);
}

void Game::Render() {
	// Render here
	window.BeginRender();

	// Use our test shader
	render.UseShaderProgram(defaultShaderProgram);

	// Test rotate the triangle
	static float angle;
	angle += 6.28f * deltaTime;

	// Set the world transform
	int uniMatWorld = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matWorld");
	glm::mat4 matViewProj = glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));

	const float playerHeight = 0.5f;
	matViewProj = glm::lookAtRH(player.GetPosition() + player.GetUp() * playerHeight, player.GetPosition() + player.GetForward(), player.GetUp()) * matViewProj;
	matViewProj = glm::perspectiveFov(90.0f, 640.0f, 480.0f, 0.1f, 100.0f) * matViewProj;

	// Upload the world transform
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, (GLfloat*)&matViewProj);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 9);
	glDisableVertexAttribArray(0);

	// Done!
	window.EndRender();
}