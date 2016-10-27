// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "04_Cube.h"
#include "Mesh.h"
#include "ErrorMessage.h"
#include "LoadShader.h"
#include "texture.h"

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

	GLuint diceTexture = texture.loadTexture("dice_texture_2.png");

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Mesh mesh;
	glm::vec3 a(-1, +1, +1);
	glm::vec3 b(+1, +1, +1);
	glm::vec3 c(+1, +1, -1);
	glm::vec3 d(-1, +1, -1);
	glm::vec3 e(-1, -1, +1);
	glm::vec3 f(-1, -1, -1);
	glm::vec3 g(+1, -1, -1);
	glm::vec3 h(+1, -1, +1);

	/*mesh.addSquare(a, b, c, d, glm::vec3(1, 0, 0), 0.25f, 0.5f, 0.0f, 0.25f);
	mesh.addSquare(b, h, g, c, glm::vec3(1, 1, 0), 0.5f, 0.75f, 0.25f, 0.5f);
	mesh.addSquare(a, e, h, b, glm::vec3(0, 1, 0), 0.25f, 0.5f, 0.25f, 0.5f);
	mesh.addSquare(d, f, e, a, glm::vec3(0, 0, 1), 0.75f, 1.0f, 0.25f, 0.5f);
	mesh.addSquare(e, f, g, h, glm::vec3(1, 0.5f, 0), 0.0f, 0.25f, 0.25f, 0.5f);
	mesh.addSquare(d, c, g, f, glm::vec3(1, 0, 1), 0.25f, 0.5f, 0.5f, 0.75f);*/

	mesh.addCircle(glm::vec3(0, -2, 0), 1, 500, glm::vec3(1, 1, 0));
	mesh.createBuffers();

	// creates a shader
	GLuint programID = Lshader.loadShaders("vertex.glsl", "fragment.glsl");



	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);

	glm::vec4 playerPosition(0, 0, 5, 1);
	float playerPitch = 0;
	float playerYaw = 0;

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);

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

		int mouseX, mouseY;
		SDL_GetRelativeMouseState(&mouseX, &mouseY);
		playerYaw -= mouseX * 0.005f;
		playerPitch -= mouseY * 0.005f;
		const float maxPitch = glm::radians(89.0f);
		if (playerPitch > maxPitch)
			playerPitch = maxPitch;
		if (playerPitch < -maxPitch)
			playerPitch = -maxPitch;

		glm::vec4 playerLook(0, 0, -1, 0);
		glm::mat4 playerRotation;
		playerRotation = glm::rotate(playerRotation, playerYaw, glm::vec3(0, 1, 0));
		playerRotation = glm::rotate(playerRotation, playerPitch, glm::vec3(1, 0, 0));
		playerLook = playerRotation * playerLook;

		/*glm::vec4 playerForward(0, 0, -1, 0);
		glm::mat4 playerForwardRotation;
		playerForwardRotation = glm::rotate(playerForwardRotation, playerYaw, glm::vec3(0, 1, 0));
		playerForward = playerForwardRotation * playerForward;*/
		glm::vec4 playerForward = playerLook;

		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		if (keyboardState[SDL_SCANCODE_W])
		{
			playerPosition += playerForward * 0.01f;
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			playerPosition -= playerForward * 0.01f;
		}

		glm::vec4 playerRight(0, 0, -1, 0);
		glm::mat4 playerRightRotation;
		playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
		playerRight = playerRightRotation * playerRight;

		if (keyboardState[SDL_SCANCODE_A])
		{
			playerPosition -= playerRight * 0.01f;
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			playerPosition += playerRight * 0.01f;
		}

		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glm::mat4 transform;
		//transform = glm::rotate(transform, SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0));
		glm::mat4 mvp = projection * view * transform;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		mesh.draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
