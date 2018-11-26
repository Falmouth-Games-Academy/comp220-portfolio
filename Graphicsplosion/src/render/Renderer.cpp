#include "stdafx.h"
#include "Renderer.h"
#include "sdl/Window.h"
#include <fstream>
#include <iostream>

#include "glm/gtc/type_ptr.hpp"

#include "SDL.h"
#include "glew.h"
#include "sdl_image.h"

GLuint vao;

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
	glEnable(GL_DEPTH_TEST);
	SDL_GL_SetSwapInterval(0); // disable vsync

	// Setup GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Test vertex array structure
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Enable the vertex attributes
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	// Init variables
	viewportSize = renderWindow.GetSize();
}

void Renderer::Shutdown() {
	// Free resources
	glDeleteVertexArrays(1, &vao);

	return;
}

void Renderer::BeginRender(bool doClear) {
	// Clear the backbuffers before render
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndRender(Window& renderWindow) {
	// Resize the renderer to the window if the size has changed
	if (viewportSize != renderWindow.GetSize()) {
		viewportSize = renderWindow.GetSize();

		glViewport(0, 0, viewportSize.x, viewportSize.y);
	}

	// Swap to the screen
	SDL_GL_SwapWindow(renderWindow.GetSdlWindow());
}

void Renderer::DrawTriangles(int startVertexIndex, int numVerticesToDraw) {
	// Bind vertex array (todo: How to not need to use glVertexAttribPointer every call...?)
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalX));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
	glVertexAttribIPointer(4, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, boneIndices));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, startVertexIndex, numVerticesToDraw);
}

void Renderer::DrawTrianglesIndexed(int startIndex, int numIndicesToDraw) {
	// Bind vertex array (todo: How to not need to use glVertexAttribPointer every call...?)
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normalX));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
	glVertexAttribIPointer(4, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, boneIndices));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

	// Draw the triangles
	glDrawElements(GL_TRIANGLES, numIndicesToDraw, GL_UNSIGNED_INT, nullptr);
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
}

void Renderer::UseVertexBuffer(const VertexBuffer* vertexBuffer) {
	// Bind the vertex buffer, if it exists (unbind otherwise)
	if (vertexBuffer) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetBufferName());
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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

ShaderProgram::ShaderProgram() : isLoaded(false) {
}

ShaderProgram::ShaderProgram(const Renderer& renderer, GLResource vertexShader, GLResource fragmentShader) {
	
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

void ShaderProgram::SetUniform(const char* uniformName, const glm::mat4& value) {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniformMatrix4fv(mapValue->second, 1, GL_FALSE, glm::value_ptr(value));
	} else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, float value) {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform1f(mapValue->second, value);
	} else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, int value) {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform1i(mapValue->second, value);
	} else {
		printf("Warning: uniform '%s' does not exist\n", uniformName);
	}
}

void ShaderProgram::SetUniform(const char* uniformName, const glm::vec3& value) {
	// Make sure the value exists first
	auto mapValue = uniforms.find(uniformName);

	if (mapValue != uniforms.end()) {
		glUniform3fv(mapValue->second, 1, glm::value_ptr(value));
	} else {
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

bool Texture::Create(Renderer& renderer, const char* textureFilename) {
	// Try to load the texture file with SDL2_image
	SDL_Surface* image;
	if (!(image = IMG_Load(textureFilename))) {
		return false;
	}

	// Generate the textures in OpenGL
	glGenTextures(1, &textureName);

	if (!textureName) {
		SDL_FreeSurface(image);
		return false;
	}

	// Copy the image data into OpenGL
	SDL_LockSurface(image);

	glBindTexture(GL_TEXTURE_2D, textureName);

	// Copy the surface data based on its format
	if (image->format->BytesPerPixel == 3) {
		if (image->format->Rmask == 0xFF) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
		} else if (image->format->Rmask == 0xFF0000) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
		} else {
			return false;
		}
	}

	// Setup parameters and mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Cleanup
	SDL_UnlockSurface(image);

	return true;
}

void Texture::Destroy() {
	// Cleanup the texture
	if (textureName) {
		glDeleteTextures(1, &textureName);
	}
}
