// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "06_Lighting.h"
#include "Mesh.h"
#include "ErrorMessage.h"
#include "LoadShader.h"
#include "Texture.h"
#include "PlayerMovement.h"
#include "Terrain.h"


// NOTE: this code is intended to illustrate usage of OpenGL.
// It is NOT intended to illustrate good coding style or naming conventions!
// Code has been copied and pasted from tutorials which use different conventions.
// In your own projects, make sure you rename identifiers to follow a consistent style.

ErrorMessage error;

LoadShader lShader;

Texture loadTexture;

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		error.custom(SDL_GetError(), "SDL_Init failed");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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

	GLuint texture = loadTexture.loadTexture("earth.jpg");

	if (texture == 0)
	{
		error.custom("loadTexture failed", ":(");
		return 1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Mesh mesh;

	Vertex p1(glm::vec3(1, -2, 1), glm::vec3(1, 1, 1));
	Vertex p2(glm::vec3(1, -2, 2), glm::vec3(1, 1, 1));
	Vertex p3(glm::vec3(0, -2, 2), glm::vec3(1,1,1));
	Vertex p4(glm::vec3(0, -2, 1), glm::vec3(1, 1, 1));
	mesh.addSquare(p3,p2,p1,p4);

	//set up terrain
	Terrain terrain(4, mesh);
	terrain.generateTerrain(300, 300, 10, 15);
	//create buffers no more adding objects after this point
	mesh.createBuffers();

	//set up player movement
	PlayerMovement player(terrain);

	//create the shader
	GLuint programID = lShader.loadShaders("vertex.glsl", "fragment.glsl");

	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");

	GLuint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);


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

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);


		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(player.getMVP()));

		glUniform3f(lightDirectionLocation, 1, 1, 1);

		mesh.draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
