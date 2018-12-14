#pragma once
#include "RenderResource.h"

class Texture {
public:
	Texture() : textureName(0) {}

	// Constructs and creates a texture from a file
	Texture(class Renderer& renderer, const char* textureFilename) {
		Create(renderer, textureFilename);
	}

	// Constructs and creates an empty texture of the specified size
	Texture(class Renderer& renderer, int width, int height) {
		Create(renderer, width, height);
	}

	~Texture() {
		// Destroy the texture (todo: might break if destructed after GL)
		Destroy();
	}

public:
	// Creates a texture from a file
	bool Create(class Renderer& renderer, const char* textureFilename);

	// Creates an empty texture
	bool Create(class Renderer& renderer, int width, int height);

	// Creates an empty depth texture
	bool CreateAsDepth(class Renderer& renderer, int width, int height);

	// Destroys the texture
	void Destroy();

public:
	// Returns the internal OpenGL texture name
	GLuint GetTextureName() const { return textureName; }

public:
	// Returns whether the texture has been successfully loaded
	bool IsLoaded() const { return textureName != 0; }

private:
	GLuint textureName;
};