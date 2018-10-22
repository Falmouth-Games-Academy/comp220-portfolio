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
	// Returns the internal GL program name
	GLuint GetGlProgram() const { return glProgram; }

private:
	// Whether this was successfully loaded
	bool isLoaded;

	GLuint glProgram;
};

// Vertex buffer
class VertexBuffer {
public:
	VertexBuffer() : bufferName(0) {}

	// Creates a vertex buffer from an optional initial vertex array
	VertexBuffer(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0) : bufferName(0) {
		Create(renderer, initialData, initialDataSize);
	}

	~VertexBuffer() {
		Destroy();
	}

public:
	// Creates a vertex buffer from an optional initial array
	void Create(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0);
	void Destroy();

	// Sets the data to a new array of vertices
	void SetData(const void* arrayData, int size);

	// Returns the internal OpenGL buffer name
	GLuint GetBufferName() const { return bufferName; }

private:
	GLuint bufferName;
};

class Texture {
public:
	Texture() : textureName(0) {}

	// Constructs and creates a texture from a file
	Texture(Renderer& renderer, const char* textureFilename) {
		Create(renderer, textureFilename);
	}

public:
	// Creates a texture from a file
	bool Create(Renderer& renderer, const char* textureFilename);

	// Returns the internal OpenGL texture name
	GLuint GetTextureName() { return textureName; }

private:
	GLuint textureName;
};

// Default vertex type
struct Vertex {
	float x, y, z;
	float r, g, b;
	float u, v;
};