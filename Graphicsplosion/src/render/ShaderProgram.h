#pragma once
#include "glm/glm.hpp"
#include "RenderResource.h"

#include <map>

class Renderer;


// Shader wrapper for OpenGL
class ShaderProgram {
public:
	// Empty shader program (call Init)
	ShaderProgram();

	// Loads and links a shader program with the given shaders
	ShaderProgram(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader);

	// Unloads/destroys the shader program
	~ShaderProgram() { Destroy(); }

public:
	// Loads and links a shader program with the given shaders
	void Create(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader);
	void Destroy();

public:
	// Attaches a shader to the program (either vertex or fragment)
	bool AttachShader(GLResource shader);

	// Links the program
	bool Link();

public:
	// SHADER MUST BE BOUND FOR THIS TO WORK! (also these are const because state machines...)
	// Sets a uniform's value
	void SetUniform(const char* uniformName, const glm::mat4& matValue) const;
	void SetUniform(const char* uniformName, const glm::vec3& vecValue) const;
	void SetUniform(const char* uniformName, int intValue) const;
	void SetUniform(const char* uniformName, float floatValue) const;

	// Sets a uniform array's values
	void SetUniforms(const char* uniformName, const glm::mat4* matValues, int numValues) const;

public:
	// Returns the location of a uniform, or -1 if this is not found
	int GetUniformLocation(const char* uniformName) const;

	// Refreshes the uniform map
	void RefreshUniformMap();

public:
	// Returns whether the shader is successfully linked and loaded
	bool IsLoaded() {
		return isLoaded;
	}

public:
	// Returns the internal GL program name
	GLuint GetGlProgram() const { return glProgram; }

private:
	// Whether this was successfully loaded
	bool isLoaded;

	// Reference to the OpenGL resource
	GLuint glProgram;

	// Uniform list
	std::map<std::string, GLuint> uniforms;
};