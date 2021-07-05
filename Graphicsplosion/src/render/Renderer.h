#pragma once
#include <map>

#include "glew.h"
#include "helpers/math.h"

#include "glm/glm.hpp"

#include "VertexFormat.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VertexBuffer.h"

enum class RenderPass {
	Shadow = 0,
	Main = 1,
};

// 3D renderer wrapper for OpenGL (by default)
class Renderer {
public:
	// Constructs but does not initialise the renderer
	Renderer() = default;

	// Constructs and initialises the renderer with a window
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
	void BeginRender(bool doClear = true, RenderPass renderPass = RenderPass::Main);
	void EndRender(class Window& renderWindow, RenderPass renderPass = RenderPass::Main);

public:
	// Draw calls
	// Draws the binded vertices as triangles
	void DrawTriangles(int startVertexIndex, int numVerticesToDraw);

	// Draws the binded indexes as triangles
	void DrawTrianglesIndexed(int startIndex, int numIndicesToDraw);

public:
	// Creates a buffer, returning its name
	GLuint CreateBuffer();
	
	// Destroys a buffer
	void DestroyBuffer(GLuint bufferName);

public:
	// Uses the supplied shader program in the next draw call
	void UseShaderProgram(const class ShaderProgram& program);

public:
	// Loads and returns a compiled shader from a source file
	GLResource LoadShaderFromSourceFile(const char* filename, GLenum glShaderType);

	// Destroys a shader
	void DestroyShader(GLResource shader);

public:
	// Sets the vertex buffer to be rendered in the following draw calls. If nullptr, the buffer is unbound
	void UseVertexBuffer(const class VertexBuffer* vertexBuffer);

	// Sets the index buffer to be rendered in the following draw calls. If nullptr, the buffer is unbound
	void UseIndexBuffer(const class IndexBuffer* indexBuffer);

public:
	// Sets the texture to be used in a draw call. If nullptr, the texture is unbound
	void UseTexture(const class Texture* texture, const class ShaderProgram* shaderProgram, const char* samplerName = "textureSampler", int textureUnit = 0);

	// Returns the shadow map texture
	class Texture* GetShadowMap();

public:
	// Returns the pixel resolution of the front buffer
	const Vec2I GetFrontBufferResolution() { return frontBufferResolution; }

private:
	// Current size of the front buffer in pixels
	Vec2I frontBufferResolution;

	// Current size of the viewport in pixels
	Vec2I viewportSize;

	// Size of the shadow map
	static const int shadowMapDefaultResolution = 1024;
	Vec2I shadowMapSize;

	// Render target textures
	static const int numRenderTargets = 2;
	Texture renderTextures[numRenderTargets];

	// Framebuffers
	GLuint frameBufferId;
	GLuint renderBufferId;

	// Post-processor
	ShaderProgram postProcessShader;
	VertexBuffer postProcessBuffer;
};