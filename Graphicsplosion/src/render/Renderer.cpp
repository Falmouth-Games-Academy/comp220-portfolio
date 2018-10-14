#include "stdafx.h"
#include "Renderer.h"
#include "sdl/Window.h"
#include <fstream>
#include <iostream>

#include "SDL.h"
#include "glew.h"

void Renderer::Init(Window& renderWindow) {
	// Create the GL context
	renderWindow.CreateGlContext();

	// Initialise OpenGL attributes (may move later)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

	// Setup GLEW
	glewExperimental = GL_TRUE;

	glewInit();
}

void Renderer::BeginRender(bool doClear) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndRender(Window& renderWindow) {
	SDL_GL_SwapWindow(renderWindow.GetSdlWindow());
}

void Renderer::UseShaderProgram(const ShaderProgram& program) {
	glUseProgram(program.GetGlProgram());
}

GLResource Renderer::LoadShaderFromSourceFile(const char* filename, GLenum glShaderType) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	if (file.fail()) {
		return GLRESOURCE_NULL; // file didn't load
	}

	// Get the file's size the old-fashioned way
	int size = 0;
	file.seekg(0, file.end);
	size = file.tellg();

	// Allocate a temporary buffer for the shader
	char* shaderString = new char[size + 1];
	shaderString[size] = '\0';

	// Read the data into the temporary buffer
	file.seekg(0, file.beg);
	file.read(shaderString, size);

	// Try to load the shader code
	GLuint result = glCreateShader(glShaderType);

	// Error checking
	if (result == 0) {
		// Free resources
		delete[] shaderString;

		return GLRESOURCE_NULL;
	}
	
	// Try and compile the shader code
	GLint compileStatus, compileLogLength;

	glShaderSource(result, 1, &shaderString, NULL);
	glCompileShader(result);
	glGetShaderiv(result, GL_COMPILE_STATUS, &compileStatus);
	glGetShaderiv(result, GL_INFO_LOG_LENGTH, &compileLogLength);

	// Free resources
	delete[] shaderString;

	// Error checking
	if (!compileStatus) {
		// Print the error message (debug)
		GLchar* infoLog = new GLchar[compileLogLength + 1];
		infoLog[compileLogLength] = '\0';

		glGetShaderInfoLog(result, compileLogLength, nullptr, infoLog);

		std::cout << "Shader compile errors/warnings: " << std::endl << infoLog << std::endl;

		// Free resources and return error
		glDeleteShader(result);

		delete[] infoLog;
		return GLRESOURCE_NULL;
	}

	return (GLResource)result;
}

ShaderProgram::ShaderProgram() : isLoaded(false) {
}

ShaderProgram::ShaderProgram(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader) {
	
}

void ShaderProgram::Init(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader) {
	// Initialise GL program
	glProgram = glCreateProgram();

	// Try and load the provided shaders
	isLoaded = true;

	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);

	Link();
}

bool ShaderProgram::AttachShader(GLResource shader) {
	glAttachShader(glProgram, shader);
	return true;
}

bool ShaderProgram::Link() {
	// Link the program!
	glLinkProgram(glProgram);

	// Check for errors and return
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(glProgram, GL_LINK_STATUS, &programSuccess);

	isLoaded = (programSuccess == GL_TRUE);
	return isLoaded;
}