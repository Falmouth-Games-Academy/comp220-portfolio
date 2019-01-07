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

// SetUniform functions simply sets a uniform after finding its location
void ShaderProgram::SetUniform(const char* uniformName, const glm::mat4& value) const {
	int location = GetUniformLocation(uniformName);

	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void ShaderProgram::SetUniform(const char* uniformName, float value) const {
	int location = GetUniformLocation(uniformName);

	if (location != -1) {
		glUniform1f(location, value);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, int value) const {
	int location = GetUniformLocation(uniformName);

	if (location != -1) {
		glUniform1i(location, value);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, const glm::vec3& value) const {
	int location = GetUniformLocation(uniformName);

	if (location != -1) {
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
}

void ShaderProgram::SetUniforms(const char* uniformName, const glm::mat4* values, int numValues) const {
	int location = GetUniformLocation(uniformName);

	if (location != -1) {
		glUniformMatrix4fv(location, numValues, GL_FALSE, (GLfloat*)values);
	}
}

int ShaderProgram::GetUniformLocation(const char* uniformName) const {
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		return mapValue->second;
	} else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
		return -1;
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