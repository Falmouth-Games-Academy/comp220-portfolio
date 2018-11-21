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

	// Shutdown cleans up resources and renderer
	void Shutdown();

public:
	// Render functions
	void BeginRender(bool doClear = true);
	void EndRender(class Window& renderWindow);

public:
	// Draw calls
	// Draws the binded triangles
	void DrawTriangles(int startVertexIndex, int numVerticesToDraw);

	// Draws the binded indexed triangles
	void DrawTrianglesIndexed(int startIndex, int numIndicesToDraw);

public:
	// Creates a buffer, returning its name
	GLuint CreateBuffer();
	
	// Destroys a buffer
	void DestroyBuffer(GLuint bufferName);

public:
	// Uses the supplied shader program in the next draw call
	void UseShaderProgram(const class ShaderProgram& program);

	GLResource LoadShaderFromSourceFile(const char* filename, GLenum glShaderType);

public:
	// Sets the vertex buffer to be rendered in a draw call. If nullptr, the buffer is unbound
	void UseVertexBuffer(const class VertexBuffer* vertexBuffer);

	// Sets the index buffer to be rendered in a draw call. If nullptr, the buffer is unbound
	void UseIndexBuffer(const class IndexBuffer* indexBuffer);

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

// Generic buffer
class GenericBuffer {
public:
	// Default constructors or destructors
	GenericBuffer() : bufferName(0) {}

	~GenericBuffer() {
		// Cleanup if applicable
		Destroy();
	}

public:
	// Creates or destroys the buffer. Overridable. By default, uses renderer.CreateBuffer renderer.DestroyBuffer
	virtual void Create(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0);

	// Destroy may be called multiple times safely
	virtual void Destroy();

public:
	// Uploads new data to the buffer
	virtual void SetData(const void* arrayData, int size) = 0;

	// Returns the internal OpenGL buffer name
	GLuint GetBufferName() const { return bufferName; }

protected:
	Renderer* renderer;

	GLuint bufferName;
};

// Vertex buffer
class VertexBuffer : public GenericBuffer {
public:
	VertexBuffer() = default;

	// Creates a buffer from an optional initial vertex array
	VertexBuffer(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0) {
		Create(renderer, initialData, initialDataSize);
	}

	~VertexBuffer() {
		Destroy();
	}

public:
	// Sets the data to a new array of vertices
	void SetData(const void* arrayData, int size);
};

// Index buffer
class IndexBuffer : public GenericBuffer {
public:
	IndexBuffer() = default;

	// Creates a buffer from an optional initial vertex array
	IndexBuffer(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0) {
		Create(renderer, initialData, initialDataSize);
	}

	~IndexBuffer() {
		Destroy();
	}

public:
	// Sets the data to a new array of vertices
	void SetData(const void* arrayData, int size);
};

class Texture {
public:
	Texture() : textureName(0) {}

	// Constructs and creates a texture from a file
	Texture(Renderer& renderer, const char* textureFilename) {
		Create(renderer, textureFilename);
	}

	~Texture() {
		// Destroy the texture (todo: might break if destructed after GL)
		Destroy();
	}

public:
	// Creates a texture from a file
	bool Create(Renderer& renderer, const char* textureFilename);

	// Destroys the texture
	void Destroy();

public:
	// Returns the internal OpenGL texture name
	GLuint GetTextureName() { return textureName; }

private:
	GLuint textureName;
};

// Default vertex type
struct Vertex {
	float x, y, z;
	float r, g, b;
	float normalX, normalY, normalZ;
	float u, v;

	unsigned __int32 boneIndices[4];
	float boneWeights[4];
};