#include "stdafx.h"
#include "ForestScene.h"
#include "Mesh.h"

ForestScene::ForestScene()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		errorSystem.createErrorMessage(SDL_GetError(), "SDL_Init failed");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		errorSystem.createErrorMessage(SDL_GetError(), "SDL_CreateWindow failed");
	}

	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		errorSystem.createErrorMessage(SDL_GetError(), "SDL_GL_CreateContext failed");
	}

	if (glewInit() != GLEW_OK)
	{
		errorSystem.createErrorMessage("glewInit failed", "Error");
	}
}

ForestScene::~ForestScene()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void ForestScene::loadTreeModel()
{
	bool tree = treeModel.loadOBJ("tree.obj", glm::vec3(1.0, 1.0, 1.0));
	if (!tree)
		errorSystem.createErrorMessage("Model loading failed.", "Error");

	treeModel.loadTextures("Textures/tree_trunk.png");
	treeModel.loadTextures("Textures/leaf.png");

	for (unsigned int i = 0; i < treeModel.modelMeshes.size(); i++)
	{
		treeModel.modelMeshes[i].createBuffers();
	}
	treeModel.checkTextures();
	treeModel.position = glm::vec3(0, -1, 0);
}

void ForestScene::run()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	//create floor plane
	Plane floor(-1, 10, "Textures/mud.png");
	// load in tree obj and textures
	//loadTreeModel();
	// create particle system
	ParticleEffectManager particleSystem(&floor, 5, 5);
	particleSystem.createMesh("Textures/leaf.png");

	// load shaders
	GLuint programID = shaders.loadShaders("vertex.glsl", "fragment.glsl");
	// set up uniforms
	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");
	GLuint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	GLuint cameraSpaceLocation = glGetUniformLocation(programID, "cameraSpace");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	glm::vec4 playerPosition(0, 0, 5, 1);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);
	Uint32 lastFrameTime = SDL_GetTicks();

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

		SDL_GetRelativeMouseState(&mouseX, &mouseY);
		playerYaw -= mouseX * mouseSensitivity;
		playerPitch -= mouseY * mouseSensitivity;
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

		glm::vec4 playerForward = playerLook;

		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		if (keyboardState[SDL_SCANCODE_W])
		{
			playerPosition += playerForward * movementMultipler;
			playerPosition.y = 0;
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			playerPosition -= playerForward * movementMultipler;
			playerPosition.y = 0;
		}

		glm::vec4 playerRight(0, 0, -1, 0);
		glm::mat4 playerRightRotation;
		playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
		playerRight = playerRightRotation * playerRight;

		if (keyboardState[SDL_SCANCODE_A])
		{
			playerPosition -= playerRight * movementMultipler;
			playerPosition.y = 0;
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			playerPosition += playerRight * movementMultipler;
			playerPosition.y = 0;
		}

		// Calculate delta time
		Uint32 currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		particleSystem.updateParticles(deltaTime);

		glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glm::mat4 transform;
		glUniform3f(lightDirectionLocation, 1, 1, 1);
		glUniform3f(cameraSpaceLocation, playerPosition.x, playerPosition.y, playerPosition.z);
		glm::mat4 mvp;

		// Render particles
		particleSystem.particleTexture.bindTexture();
		for (int i = 0; i < particleSystem.particles.size(); i++)
		{
			transform = glm::mat4();
			if (particleSystem.particles[i].lifeSpan > 0)
			{
				transform = glm::translate(transform, particleSystem.particles[i].position);
				if (particleSystem.particles[i].position.y > floor.getY() + 1)
				{
					transform = glm::rotate(transform, sin(currentTime / 400.0f), glm::vec3(0, 0, 1));
					transform = glm::rotate(transform, sin(currentTime / 400.0f), glm::vec3(1, 0, 0));
				}

				mvp = projection * view * transform;
				glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
				particleSystem.particleMesh.draw();
			}
		}

		


		// Render floor
		transform = glm::mat4();
		mvp = projection * view * transform;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
		floor.texture.bindTexture();
		floor.mesh.draw();
			
		
		/*// Render trees
		transform = glm::mat4();
		transform = glm::translate(transform, treeModel.position);
		transform = glm::scale(transform, glm::vec3(4, 4, 4));
		mvp = projection * view * transform;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		for (unsigned int i = 0; i < treeModel.modelTextures.size(); i++)
		{
			treeModel.modelTextures[i].bindTexture();
			treeModel.modelMeshes[i].draw();
		}*/
		
		SDL_GL_SwapWindow(window);
	}


}
