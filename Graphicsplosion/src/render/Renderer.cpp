#include "stdafx.h"
#include "Renderer.h"
#include <fstream>

Renderer::Renderer() {
	return;// do nothing lawl
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
	GLint compileStatus;

	glShaderSource(result, 1, &shaderString, NULL);
	glCompileShader(result);
	glGetShaderiv(result, GL_COMPILE_STATUS, &compileStatus);

	// Free resources
	delete[] shaderString;

	// Error checking
	if (!compileStatus) {

		// Error occurred - free resources
		glDeleteShader(result);
		return GLRESOURCE_NULL;
	}

	return (GLResource)result;
}
