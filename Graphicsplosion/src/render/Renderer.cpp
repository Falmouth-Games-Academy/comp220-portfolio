#include "stdafx.h"
#include "Renderer.h"
#include "sdl/Window.h"
#include "IndexBuffer.h"
#include "main/Time.h"
#include <fstream>
#include <iostream>

#include "glm/gtc/type_ptr.hpp"

#include "SDL.h"
#include "glew.h"

void Renderer::Init(Window& renderWindow) {
	// Initialise OpenGL attributes (may move later)
	//GL_ARB_framebuffer_object
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // set the GL context major version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); // set the GL context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE); // enable double-buffering

	// Create the GL context
	renderWindow.CreateGlContext();

	// Do additional setup
	glEnable(GL_DEPTH_TEST); // enable Z-buffer
	//glEnable(GL_BLEND); // enable alpha blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_GL_SetSwapInterval(0); // disable vsync

	// Setup GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Init variables
	viewportSize = renderWindow.GetSize();
	frontBufferResolution = viewportSize;

	// Init render textures
	renderTextures[0].Create(*this, viewportSize.x, viewportSize.y);
	renderTextures[1].CreateAsDepth(*this, viewportSize.x, viewportSize.y);

	// Create the depth buffers
	glGenRenderbuffers(1, &renderBufferId);

	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, viewportSize.x, viewportSize.y);

	// Create and bind the frame buffers
	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	// Attach the depth buffer to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

	// Attach the texture to the framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextures[0].GetTextureName(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Error creating and attaching frame buffer!\n");
	}

	// Create the postprocessor
	static PostProcessVertex postProcessorPlane[6] {
		glm::vec2(-1.0f, -1.0f), glm::vec2(-1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
		glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, -1.0f)
	};

	postProcessShader.Create(*this, LoadShaderFromSourceFile("src/shaders/vertexPassthrough.txt", GL_VERTEX_SHADER), LoadShaderFromSourceFile("src/shaders/fragmentPostProcess.txt", GL_FRAGMENT_SHADER));
	postProcessBuffer.Create(*this, VertexFormat(&PostProcessVertex::position), postProcessorPlane, sizeof (postProcessorPlane));
}

void Renderer::Shutdown() {
	return;
}

void Renderer::BeginRender(bool doClear, RenderPass renderPass) {
	// Resize the viewport
	if (renderPass == RenderPass::Main) {
		glViewport(0, 0, viewportSize.x, viewportSize.y);
	}
	else {
		glViewport(0, 0, 640, 480);
	}

	// Attach renderbuffer stuff....!?!!?11
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);

	if (renderPass == RenderPass::Main) {
		// Attach the depth buffer to the framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferId);

		// Attach the texture to the framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTextures[0].GetTextureName(), 0);
	} else if (renderPass == RenderPass::Shadow) {
		// Attach the shadow depth texture to the framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, renderTextures[1].GetTextureName(), 0);

		// Attach a blank colour attachment to the framebuffer
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
	}

	// Set render-to-texture
	glEnable(GL_DEPTH_TEST);

	// Clear the backbuffers before render if desired
	if (doClear) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Renderer::EndRender(Window& renderWindow, RenderPass renderPass) {
	// Resize the renderer to the window if the size has changed
	if (viewportSize != renderWindow.GetSize()) {
		viewportSize = renderWindow.GetSize();

		// Recreate the postprocessor texture
		renderTextures[0].Destroy();
		renderTextures[0].Create(*this, viewportSize.x, viewportSize.y);

		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, viewportSize.x, viewportSize.y);

		glGenFramebuffers(1, &frameBufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	}

	// Perform post-processing
	if (renderPass == RenderPass::Main) {
		// Bind the post-process frame buffer
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Use the postprocessor texture
		UseShaderProgram(postProcessShader);
		UseVertexBuffer(&postProcessBuffer);
		UseTexture(&renderTextures[0], &postProcessShader, "colorSampler");
		UseTexture(&renderTextures[1], &postProcessShader, "shadowSampler");

		//postProcessShader.SetUniform("time", (float)Time::GetTime());

		// Draw it!
		DrawTriangles(0, 6);

		// Swap to the screen
		SDL_GL_SwapWindow(renderWindow.GetSdlWindow());
	}
}

void Renderer::DrawTriangles(int startVertexIndex, int numVerticesToDraw) {
	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, startVertexIndex, numVerticesToDraw);
}

void Renderer::DrawTrianglesIndexed(int startIndex, int numIndicesToDraw) {
	// Draw the triangles
	glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, &((int*)nullptr)[startIndex]);
}

GLuint Renderer::CreateBuffer() {
	// Return a new GL buffer
	GLuint buffer = 0;

	glGenBuffers(1, &buffer);
	return buffer;
}

void Renderer::DestroyBuffer(GLuint bufferName) {
	glDeleteBuffers(1, &bufferName);
}

void Renderer::UseShaderProgram(const ShaderProgram& program) {
	glUseProgram(program.GetGlProgram());

	// Temporary: bind shadow map
	UseTexture(&renderTextures[1], &program, "shadowSampler", 1);
}

void Renderer::UseVertexBuffer(const VertexBuffer* vertexBuffer) {
	// Bind the vertex buffer, if it exists (unbind otherwise)
	if (vertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetBufferName());
		glBindVertexArray(vertexBuffer->GetVAO());
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Renderer::UseIndexBuffer(const IndexBuffer* indexBuffer) {
	// Bind the index buffer, if it exists (unbind otherwise)
	if (indexBuffer) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetBufferName());
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void Renderer::UseTexture(const Texture* texture, const ShaderProgram* shaderProgram, const char* samplerName, int textureUnit) {
	if (texture) {
		// Temporary: get the texture sampler uniform
		int uniTexture = glGetUniformLocation(shaderProgram->GetGlProgram(), samplerName);
		
		if (uniTexture != -1) {
			// Bind the texture to the sampler
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, texture->GetTextureName());
			glBindSampler(textureUnit, uniTexture);
			glUniform1i(uniTexture, textureUnit);
		}
	} else {
		// Otherwise bind a blank texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

Texture* Renderer::GetShadowMap() {
	return &renderTextures[1];
}

GLResource Renderer::LoadShaderFromSourceFile(const char* filename, GLenum glShaderType) {
	// Try to open the shader file
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

void GenericBuffer::Create(Renderer& renderer, const void* initialData, int initialDataSize) {
	// Initialise with a new buffer
	bufferName = renderer.CreateBuffer();

	// Initialise it with the data if it was supplied
	if (initialData) {
		SetData(initialData, initialDataSize);
	}

	// Connect this to the renderer (todo: don't do I guess)
	this->renderer = &renderer;
}

void GenericBuffer::Destroy() {
	// Cleanup
	if (renderer) {
		renderer->DestroyBuffer(bufferName);
	}

	bufferName = 0;
}

void VertexBuffer::Create(Renderer& renderer, const VertexFormat& vertexFormat, const void* initialData, int initialDataSize) {
	// Create the vertex array object
	glGenVertexArrays(1, &vaoName);
	glBindVertexArray(vaoName);

	// Create the buffer as per usual
	GenericBuffer::Create(renderer, initialData, initialDataSize);

	// Set attribute pointers
	for (const VertexFormat::VertexAttribute& attribute : vertexFormat.GetAttributeList()) {
		// Send this attribute to OpenGL
		if (attribute.type == GL_INT || attribute.type == GL_UNSIGNED_INT || attribute.type == GL_BYTE || attribute.type == GL_UNSIGNED_BYTE) {
			glVertexAttribIPointer(attribute.location, attribute.size, attribute.type, vertexFormat.GetVertexSize(), (const void*)attribute.offset);
		} else {
			glVertexAttribPointer(attribute.location, attribute.size, attribute.type, GL_FALSE, vertexFormat.GetVertexSize(), (const void*)attribute.offset);
		}

		// Enable it!
		glEnableVertexAttribArray(attribute.location);
	}

	// DONT TOUCH IT
	glBindVertexArray(0);
}

void VertexBuffer::Destroy()  {
	// Destroy base buffer
	GenericBuffer::Destroy();

	// Destroy VAO
	if (renderer) {
		renderer->DestroyBuffer(vaoName);
	}

	vaoName = 0;
}

void VertexBuffer::SetData(const void* arrayData, int size) {
	// Vertex buffers: Upload the data to the GL array buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferName);
	glBufferData(GL_ARRAY_BUFFER, size, arrayData, GL_STATIC_DRAW);
}

void IndexBuffer::SetData(const void* arrayData, int size) {
	// Index buffers: Upload the data to the GL element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferName);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, arrayData, GL_STATIC_DRAW);
}