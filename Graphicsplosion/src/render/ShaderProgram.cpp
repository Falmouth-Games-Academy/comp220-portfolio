#include "stdafx.h"
#include "ShaderProgram.h"

#include "glm/gtc/type_ptr.hpp"

ShaderProgram::ShaderProgram() : isLoaded(false) {
}

ShaderProgram::ShaderProgram(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader) : isLoaded(false) {
	// Just call the main Create function
	Create(renderer, vertexShader, fragmentShader);
}

void ShaderProgram::Create(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader) {
	// Initialise the GL program
	glProgram = glCreateProgram();

	// Try and load the provided shaders
	isLoaded = true;

	glAttachShader(glProgram, vertexShader);
	glAttachShader(glProgram, fragmentShader);

	// Link the shaders
	Link();

	// Refresh uniforms
	RefreshUniformMap();
}

void ShaderProgram::Destroy() {
	// Delete the program if it hasn't already been deleted
	if (glProgram) {
		glDeleteProgram(glProgram);

		glProgram = 0;
	}
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

void ShaderProgram::SetUniform(const char* uniformName, const glm::mat4& value) const {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniformMatrix4fv(mapValue->second, 1, GL_FALSE, glm::value_ptr(value));
	}
	else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, float value) const {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform1f(mapValue->second, value);
	}
	else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, int value) const {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform1i(mapValue->second, value);
	}
	else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, const glm::vec3& value) const {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform3fv(mapValue->second, 1, glm::value_ptr(value));
	}
	else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::RefreshUniformMap() {
	// Clear the uniform map
	uniforms.clear();

	// Iterate every uniform and add it to the map
	GLint numUniforms = 0;
	glGetProgramiv(glProgram, GL_ACTIVE_UNIFORMS, &numUniforms);

	printf("Loading %i uniforms:", numUniforms);

	for (int i = 0; i < numUniforms; i++) {
		const int maxUniformNameLength = 64;
		GLchar uniformName[maxUniformNameLength + 1] = "";
		GLint size = maxUniformNameLength; // allow null terminator
		GLint uniformSize;
		GLenum uniformType;
		GLuint uniformLocation = 0;

		glGetActiveUniform(glProgram, (GLuint)i, size, nullptr, &uniformSize, &uniformType, uniformName);
		uniformLocation = glGetUniformLocation(glProgram, uniformName);

		printf("%s, ", uniformName);

		uniforms.insert(std::make_pair(std::string(uniformName), uniformLocation));
	}

	printf("\n");
}