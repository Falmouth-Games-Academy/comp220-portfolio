// 01_FirstOpenGL.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Portfolio_Main.h"
#include "Mesh.h"

void showErrorMessage(const char* message, const char* title)
{
	// Note: this is specific to Windows, and would need redefining to work on Mac or Linux
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

bool compileShader(GLuint shaderId, const std::string& shaderFileName)
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
		showErrorMessage(shaderFileName.c_str(), "File not found");
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
		showErrorMessage(errorMessage.data(), shaderFileName.c_str());
	}

	return (result != GL_FALSE);
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

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
		showErrorMessage(errorMessage.data(), "glLinkProgram error");
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

GLuint loadTexture(const std::string& fileName)
{
	SDL_Surface* textureSurface = IMG_Load(fileName.c_str());

	if (textureSurface == nullptr)
	{
		showErrorMessage(SDL_GetError(), "IMG_Load failed");
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
		showErrorMessage("Invalid pixel format", ":(");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(textureSurface);
	return textureId;
}

int main(int argc, char* args[])
{

	int windowWidth = 1000;
	int windowHeight = 1000;


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		showErrorMessage(SDL_GetError(), "SDL_Init failed");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("Portfolio Project - Alli", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		showErrorMessage(SDL_GetError(), "SDL_CreateWindow failed");
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		showErrorMessage(SDL_GetError(), "SDL_GL_CreateContext failed");
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		showErrorMessage("glewInit failed", ":(");
		return 1;
	}

	GLuint diceTexture = loadTexture("dice_texture_2.png");

	if (diceTexture == 0)
	{
		showErrorMessage("loadTexture failed", ":(");
		return 1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	Mesh mesh;

	/* CUBE 
	glm::vec3 a(-1, +1, +1);
	glm::vec3 b(+1, +1, +1);
	glm::vec3 c(+1, +1, -1);
	glm::vec3 d(-1, +1, -1);
	glm::vec3 e(-1, -1, +1);
	glm::vec3 f(-1, -1, -1);
	glm::vec3 g(+1, -1, -1);
	glm::vec3 h(+1, -1, +1);

	mesh.addSquare(a, b, c, d, glm::vec3(1, 0, 0), 0.25f, 0.5f, 0.0f, 0.25f);
	mesh.addSquare(b, h, g, c, glm::vec3(1, 1, 0), 0.5f, 0.75f, 0.25f, 0.5f);
	mesh.addSquare(a, e, h, b, glm::vec3(0, 1, 0), 0.25f, 0.5f, 0.25f, 0.5f);
	mesh.addSquare(d, f, e, a, glm::vec3(0, 0, 1), 0.75f, 1.0f, 0.25f, 0.5f);
	mesh.addSquare(e, f, g, h, glm::vec3(1, 0.5f, 0), 0.0f, 0.25f, 0.25f, 0.5f);
	mesh.addSquare(d, c, g, f, glm::vec3(1, 0, 1), 0.25f, 0.5f, 0.5f, 0.75f);

	//mesh.addCircle(glm::vec3(0, -2, 0), 1, 500, glm::vec3(1, 1, 0));
	*/
	mesh.addCylinder(glm::vec3(0, -2, 0), 1, 24, -2, glm::vec3(1, 0, 0));
	

	PerlinNoise perlinNoise;

	// Used for third dimension of perlin noise
	int z = 0;
	int seed = SDL_GetTicks() / 100;
	int seed2 = rand() % 256;

	perlinNoise.GenerateNoise(seed);


	int chunkSize = 1725; // Max 1725 squares ~3M
	int noiseMax = 3;
	int noiseMin = 0;

	// Amplification(the lower the number the higher the amplification)
	float noiseAmplification = 100.0;

	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			double perlinResult = perlinNoise.noise((x / noiseAmplification), (y / noiseAmplification), z);

			//Normalize values
			perlinResult = (char)((perlinResult - noiseMin) * (255 / (noiseMax - noiseMin)));;


			//Generate squares next to each other with different z position values
			//mesh.addSquare(glm::vec3(x, perlinResult, y), glm::vec3(x + 1, perlinResult, y), glm::vec3(x + 1, perlinResult + 1, y), glm::vec3(x, perlinResult + 1, y), // SideWays Squares
			mesh.addSquare(glm::vec3(x, y, perlinResult), glm::vec3(x + 1, y, perlinResult), glm::vec3(x + 1, y + 1, perlinResult), glm::vec3(x, y + 1, perlinResult), // Flat Squares

				// Square Colour
				//glm::vec3(1, 0.5, 0), //Orange
				//glm::vec3(sin(perlinResult), cos(perlinResult), tan(perlinResult)), //Rainbow Red/White
				glm::vec3(perlinResult / 80 ,perlinResult / 35, perlinResult / 700 ), // Grassy texture
				//glm::vec3(sin(perlinResult) / 80 ,sin(perlinResult / 35), sin(perlinResult) / 100 ), // Grassy texture
				//glm::vec3(perlinResult / 10, perlinResult / 30, perlinResult / 75), // Grassy texture

				// UV maps
				0, 0, 0, 0);

			
		}
	}
	mesh.createBuffers();
	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");
	GLuint mvpLocation = glGetUniformLocation(programID, "mvp");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	glm::vec4 playerPosition(0, 50, 50, 1);
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
		// Mouse sensitivity
		playerYaw -= mouseX * 0.005f;
		playerPitch -= mouseY * 0.005f;

		// Max up & down view distance
		const float maxPitch = glm::radians(89.0f);
		if (playerPitch > maxPitch)
			playerPitch = maxPitch;
		if (playerPitch < -maxPitch)
			playerPitch = -maxPitch;

		// Forwards and back movement controls
		glm::vec4 playerLook(0, 0, -1, 0);
		glm::mat4 playerRotation;
		playerRotation = glm::rotate(playerRotation, playerYaw, glm::vec3(0, 1, 0));
		playerRotation = glm::rotate(playerRotation, playerPitch, glm::vec3(1, 0, 0));
		playerLook = playerRotation * playerLook;

		glm::vec4 playerForward = playerLook;

		
		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		if (keyboardState[SDL_SCANCODE_W])
		{
			playerPosition += playerForward * 0.1f;
			// Speed modifier
			if (keyboardState[SDL_SCANCODE_LSHIFT])
			{
				playerPosition += playerForward * 2.0f;
			}
		}
		if (keyboardState[SDL_SCANCODE_S])
		{
			playerPosition -= playerForward * 0.1f;
			// Speed modifier
			if (keyboardState[SDL_SCANCODE_LSHIFT])
			{
				playerPosition -= playerForward * 2.0f;
			}
		}


		// For left and right movement
		glm::vec4 playerRight(0, 0, -1, 0);
		glm::mat4 playerRightRotation;
		playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
		playerRight = playerRightRotation * playerRight;

		if (keyboardState[SDL_SCANCODE_A])
		{
			playerPosition -= playerRight * 0.1f;
			if (keyboardState[SDL_SCANCODE_LSHIFT])
			{
				playerPosition -= playerRight * 2.0f;
			}
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			playerPosition += playerRight * 0.1f;
			if (keyboardState[SDL_SCANCODE_LSHIFT])
			{
				playerPosition += playerRight * 2.0f;
			}
		}

		

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));

		// Render Distance
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10000.0f);

		glm::mat4 transform;
		transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(1, 0, 0));  // Rotate by 90 to make the level flat
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
