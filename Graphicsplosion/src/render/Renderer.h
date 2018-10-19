#pragma once
#include "glew.h"
#include "helpers/math.h"

// lazy wrapper for unsafe GLuint type
enum GLResource : GLuint {
	GLRESOURCE_NULL = 0,
};

// 3D renderer wrapper for OpenGL (by default)
class Renderer {
public:
	Renderer() = default;
	Renderer(class Window& renderWindow) {
		Init(renderWindow);
	}

public:
	// Init function for deferred creation
	void Init(class Window& renderWindow);

public:
	// Render functions
	void BeginRender(bool doClear = true);
	void EndRender(class Window& renderWindow);

public:
	// Creates a buffer, returning its name
	GLuint CreateBuffer();

public:
	void UseShaderProgram(const class ShaderProgram& program);
	void UseVertexBuffer(const class VertexBuffer& vertexBuffer);

	GLResource LoadShaderFromSourceFile(const char* filename, GLenum glShaderType);

private:
	Vec2I viewportSize;
};

// Shader wrapper for OpenGL
class ShaderProgram {
public:
	// Empty shader program (call Init)
	ShaderProgram();

	// Loads and links a shader program with the given shaders
	ShaderProgram(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader);

public:
	void Init(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader);

public:
	// Attaches a shader to the program (either vertex or fragment)
	bool AttachShader(GLResource shader);

	// Links the program
	bool Link();

public:
	// Returns whether the shader is successfully linked and loaded
	bool IsLoaded() {
		return isLoaded;
	}

public:
	GLuint GetGlProgram() const {
		return glProgram;
	}

private:
	bool isLoaded;

	GLuint glProgram;
};

// Vertex buffer
class VertexBuffer {
public:
	VertexBuffer() : bufferName(0) {}

	VertexBuffer(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0) : bufferName(0) {
		Create(renderer, initialData, initialDataSize);
	}

	~VertexBuffer() {
		Destroy();
	}

public:
	void Create(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0);
	void Destroy();

	void SetData(const void* arrayData, int size);

	GLuint GetBufferName() const {
		return bufferName;
	}

private:
	GLuint bufferName;
};

// Default vertex type
struct Vertex {
	float x, y, z;
	float r, g, b;
};