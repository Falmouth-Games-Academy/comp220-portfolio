#include "stdafx.h"
#include "ForestScene.h"
#include "Mesh.h"

ForestScene::ForestScene()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		errorSystem.showErrorMessage(SDL_GetError(), "SDL_Init failed");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		errorSystem.showErrorMessage(SDL_GetError(), "SDL_CreateWindow failed");
	}

	glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		errorSystem.showErrorMessage(SDL_GetError(), "SDL_GL_CreateContext failed");
	}

	if (glewInit() != GLEW_OK)
	{
		errorSystem.showErrorMessage("glewInit failed", "Error");
	}

	texture = loadTexture("treeTexture.png");

	if (texture == 0)
	{
		errorSystem.showErrorMessage("loadTexture failed", "Error");
	}

}

ForestScene::~ForestScene()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

bool ForestScene::compileShader(GLuint shaderId, const std::string& shaderFileName)
{
	// Read the source code from the file
	std::string shaderSource;
	std::ifstream sourceStream(shaderFileName, std::ios::in);
	if (sourceStream.is_open())
	{
		std::stringstream buffer;
		buffer << sourceStream.rdbuf();
		shaderSource = buffer.str();
		sourceStream.close();
	}
	else
	{
		errorSystem.showErrorMessage(shaderFileName.c_str(), "File not found");
		return false;
	}

	// Compile the shader
	const char* sourcePointer = shaderSource.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	// Check the results of compilation
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1)
	{
		// Display the compilation log
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, errorMessage.data());
		errorSystem.showErrorMessage(errorMessage.data(), shaderFileName.c_str());
	}

	return (result != GL_FALSE);
}

GLuint ForestScene::loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

	// Create the shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShaderId, vertex_file_path);

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShaderId, fragment_file_path);

	// Link the program
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, errorMessage.data());
		errorSystem.showErrorMessage(errorMessage.data(), "glLinkProgram error");
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

GLuint ForestScene::loadTexture(const std::string& fileName)
{
	SDL_Surface* textureSurface = IMG_Load(fileName.c_str());

	if (textureSurface == nullptr)
	{
		errorSystem.showErrorMessage(SDL_GetError(), "IMG_Load failed");
		return 0;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int format;
	if (textureSurface->format->BytesPerPixel == 3)
	{
		format = GL_RGB;
	}
	else if (textureSurface->format->BytesPerPixel == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		errorSystem.showErrorMessage("Invalid pixel format", ":(");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(textureSurface);
	return textureId;
}

void ForestScene::run()
{
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Mesh mesh;
	mesh.loadOBJ("tree.obj");
	
	mesh.createBuffers();

	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");

	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");

	GLuint lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

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
			playerPosition += playerForward * 0.001f;
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			playerPosition -= playerForward * 0.001f;
		}

		glm::vec4 playerRight(0, 0, -1, 0);
		glm::mat4 playerRightRotation;
		playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
		playerRight = playerRightRotation * playerRight;

		if (keyboardState[SDL_SCANCODE_A])
		{
			playerPosition -= playerRight * 0.001f;
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			playerPosition += playerRight * 0.001f;
		}

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glm::mat4 transform;
		//transform = glm::rotate(transform, SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0));
		glm::mat4 mvp = projection * view * transform;
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform3f(lightDirectionLocation, 1, 1, 1);

		mesh.draw();

		SDL_GL_SwapWindow(window);
	}


}