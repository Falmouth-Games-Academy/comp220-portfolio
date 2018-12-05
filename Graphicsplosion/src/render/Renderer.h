#pragma once
#include "glew.h"
#include "helpers/math.h"

#include <map>

#include "glm/glm.hpp"

#include "render/VertexFormat.h" // todo possibly remove

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
	void BeginRender(bool doClear = true, int passIndex = 0);
	void EndRender(class Window& renderWindow, int passIndex = 0);

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

public:
	// Sets the texture to be used in a draw call. If nullptr, the texture is unbound
	void UseTexture(const class Texture* texture, const class ShaderProgram* shaderProgram, const char* samplerName = "textureSampler", int textureUnit = 0);

	// Returns the shadow map texture
	class Texture* GetShadowMap();

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
	// SHADER MUST BE BOUND FOR THIS TO WORK! (also these are const because state machines...)
	// Sets a uniform's value
	void SetUniform(const char* uniformName, const glm::mat4& matValue) const;
	void SetUniform(const char* uniformName, const glm::vec3& vecValue) const;
	void SetUniform(const char* uniformName, int intValue) const;
	void SetUniform(const char* uniformName, float floatValue) const;

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
	VertexBuffer() : vaoName(0) {};

	// Creates a buffer from an optional initial vertex array
	VertexBuffer(Renderer& renderer, const class VertexFormat& vertexFormat, const void* initialData = nullptr, int initialDataSize = 0) {
		Create(renderer, vertexFormat, initialData, initialDataSize);
	}

	~VertexBuffer() {
		Destroy();
	}

public:
	// Creates the buffer
	void Create(Renderer& renderer, const class VertexFormat& vertexFormat, const void* initialData = nullptr, int initialDataSize = 0);
	// Destroys the buffer
	void Destroy() override;

	// Sets the buffer data to a new array of vertices
	void SetData(const void* arrayData, int size);

	GLuint GetVAO() const { return vaoName; }

private:
	// GL buffer for the VAO
	GLuint vaoName;
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

	// Constructs and creates an empty texture of the specified size
	Texture(Renderer& renderer, int width, int height) {
		Create(renderer, width, height);
	}

	~Texture() {
		// Destroy the texture (todo: might break if destructed after GL)
		Destroy();
	}

public:
	// Creates a texture from a file
	bool Create(Renderer& renderer, const char* textureFilename);

	// Creates an empty texture
	bool Create(Renderer& renderer, int width, int height);

	// Creates an empty depth texture
	bool CreateAsDepth(Renderer& renderer, int width, int height);

	// Destroys the texture
	void Destroy();

public:
	// Returns the internal OpenGL texture name
	GLuint GetTextureName() const { return textureName; }

private:
	GLuint textureName;
};