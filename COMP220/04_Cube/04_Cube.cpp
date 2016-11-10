// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "04_Cube.h"
#include "Mesh.h"
#include "ErrorMessage.h"
#include "LoadShader.h"
#include "texture.h"
#include "PlayerMovement.h"

// NOTE: this code is intended to illustrate usage of OpenGL.
// It is NOT intended to illustrate good coding style or naming conventions!
// Code has been copied and pasted from tutorials which use different conventions.
// In your own projects, make sure you rename identifiers to follow a consistent style.

int main(int argc, char* args[])
{
	// defines an instance of the error message class
	ErrorMessage error;

	//defines an instance of the texture class
	Texture texture;

	//defines an instance of the load shader class
	LoadShader Lshader;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		error.custom(SDL_GetError(), "SDL_Init failed");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		error.custom(SDL_GetError(), "SDL_CreateWindow failed");
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		error.custom(SDL_GetError(), "SDL_GL_CreateContext failed");
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		error.custom("glewInit failed", ":(");
		return 1;
	}

	GLuint diceTexture = texture.loadTexture("dice_texture_2.png");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	PlayerMovement player;

	Mesh mesh;
	mesh.generateTerrain(2500, 2500, 150 ,15);
	/*
	noise::Perlin perlinNoise;
	glm::vec3 a(+1, perlinNoise.noise(1, -1, 0), -1);
	glm::vec3 b(+1, perlinNoise.noise(1, 1, 0), 1);
	glm::vec3 c(-1, perlinNoise.noise(-1, -1, 0), -1);
	glm::vec3 d(-1, perlinNoise.noise(-1, 1, 0), 1);
	srand(9385746);
	float random1 = float(rand() % 255) / 255.0f;
	float random2 = float(rand() % 255) / 255.0f;
	float random3 = float(rand() % 255) / 255.0f;
	glm::vec3 colour(random1, random2, random3);
	glm::vec2 texture1(0,0);
	mesh.addTriangle(a, c, b, colour, texture1, texture1, texture1);
	*/


	mesh.addCircle(glm::vec3(0, -2, 0), 1, 500, glm::vec3(1, 1, 0));
	mesh.createBuffers();

	// creates a shader
	GLuint programID = Lshader.loadShaders("vertex.glsl", "fragment.glsl");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);


	// set up for player movement class
	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");




	//start main loop
	bool running = true;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}
		}

		player.tick();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);


		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(player.getMVP()));

		mesh.draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
