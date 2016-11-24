#include "stdafx.h"
#include "cave.h"
#include "Mesh.h"


void showError(const char* message, const char* title)
{
	MessageBoxA(nullptr, message, title, MB_OK | MB_ICONERROR);
}

bool compileShader(GLuint shaderId, const std::string& shaderFileName)
{
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
		showError(shaderFileName.c_str(), "File not found");
		return false;
	}

	const char* sourcePointer = shaderSource.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1)
	{
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, errorMessage.data());
		showError(errorMessage.data(), shaderFileName.c_str());
	}

	return (result != GL_FALSE);
}

GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShaderId, vertex_file_path);

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShaderId, fragment_file_path);

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	GLint result = GL_FALSE;
	int infoLogLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 1) {
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, errorMessage.data());
		showError(errorMessage.data(), "glLinkProgram error");
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return programId;
}

/*Lighting for the demo*/
int light()
{
	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");
	GLuint lighting = glGetUniformLocation(programID, "lightDirection");
	glUniform3f(lighting, 1, 1, 1);
}

/*Loading textures to texture the walls*/
GLuint getTexture(const std::string& fileName)
{
	SDL_Surface* textureImage = IMG_Load(fileName.c_str());

	if (textureImage == nullptr)
	{
		showError(SDL_GetError(), "IMG_Load failed");
		return 0;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int form;
	if (textureImage->form->BytesPerPixel == 3)
	{
		form = GL_RGB;
	}
	else if (textureImage->form->BytesPerPixel == 4)
	{
		form = GL_RGBA;
	}
	else
	{
		showError("Invalid pixel form", ":(");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, form, textureImage->w, textureImage->h, 0, form, GL_UNSIGNED_BYTE, textureImage->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(textureImage);
	return textureId;
}

/*This is me using cellular automata to go through a grid and define is a cell is a wall if so it will load a mesh*/
int caveGen(wall, nonWall)
{
	for (int x = 0; x < 10; x++)
	{
		x = 1 + (int)(10.0 * (rand() / (RAND_MAX + 1.0)));
		if (x > 5)
		{
			mesh mesh;
			mesh.wall(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 0, 0, 0, 0);
			mesh.createBuffers();
		}
		else
		{
			int nonWall;
		}

		for (int y = 0; y < 10; y++) //creating a grid
		{
			y = 1 + (int)(10.0 * (rand() / (RAND_MAX + 1.0)));
			if (y > 5)
			{
				mesh mesh;
				mesh.wall(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), 0, 0, 0, 0);
				mesh.createBuffers();
			}
			else
			{
				int nonWall;
			}

		}
	}
}

/*main function which currently still has played movement*/
int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		showError(SDL_GetError(), "SDL_Init did not work");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("My first OpenGL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == nullptr)
	{
		showError(SDL_GetError(), "SDL_CreateWindow did not work");
		return 1;
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (glContext == nullptr)
	{
		showError(SDL_GetError(), "SDL_GL_CreateContext did not work");
		return 1;
	}

	if (glewInit() != GLEW_OK)
	{
		showError(":(", "Invalid pixel form");
		return 1;
	}

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = loadShaders("vertex.glsl", "fragment.glsl");

	GLuint mvpPosition = glGetUniformLocation(programID, "mvp");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	glm::vec4 plyrPos(0, 0, 4, 1);
	float plyrPitch = 0;
	float plyrYaw = 0;

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

		int Xmouse, Ymouse;
		SDL_GetRelativeMouseState(&Xmouse, &Ymouse);
		plyrYaw -= Xmouse * 0.005f;
		plyrPitch -= Ymouse * 0.005f;
		const float maxPitch = glm::radians(89.0f);
		if (plyrPitch > maxPitch)
			plyrPitch = maxPitch;
		if (plyrPitch < -maxPitch)
			plyrPitch = -maxPitch;

		glm::vec4 plyrView(0, 0, -1, 0);
		glm::mat4 plyrRotation;
		plyrRotation = glm::rotate(plyrRotation, plyrYaw, glm::vec3(0, 1, 0));
		plyrRotation = glm::rotate(plyrRotation, plyrPitch, glm::vec3(1, 0, 0));
		plyrView = plyrRotation * plyrView;
		glm::vec4 plyrFwrd = plyrView;

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);
		if (keyState[SDL_SCANCODE_W])
		{
			plyrPos += plyrFwrd * 0.001f;
		}
		if (keyState[SDL_SCANCODE_S])
		{
			plyrPos -= plyrFwrd * 0.001f;
		}

		glm::vec4 plyrRight(0, 0, -1, 0);
		glm::mat4 plyrRightRotation;
		plyrRightRotation = glm::rotate(plyrRightRotation, plyrYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
		plyrRight = plyrRightRotation * plyrRight;

		if (keyState[SDL_SCANCODE_A])
		{
			plyrPos -= plyrRight * 0.001f;
		}
		if (keyState[SDL_SCANCODE_D])
		{
			plyrPos += plyrRight * 0.001f;
		}

		glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		glm::mat4 sight = glm::lookAt(glm::vec3(plyrPos), glm::vec3(plyrPos + plyrView), glm::vec3(0, 1, 0));
		glm::mat4 project = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glm::mat4 transform;
		glm::mat4 mvp = project * sight * transform;
		glUniformMatrix4fv(mvpPosition, 1, GL_FALSE, glm::value_ptr(mvp));

		GLuint wallTexture = getTexture("floor.jpg");

		if (wallTexture)
		{
			showError("getTexture did not work", ":(");
			return 1;
		}


		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}