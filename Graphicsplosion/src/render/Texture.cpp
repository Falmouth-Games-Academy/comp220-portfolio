#include "stdafx.h"
#include "Renderer.h"
#include "Texture.h"

#include "SDL.h"
#include "sdl_image.h"

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
		}
		else if (image->format->Rmask == 0xFF0000) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);
		}
		else {
			return false;
		}
	}
	else if (image->format->BytesPerPixel == 4) {
		// Texture has alpha channel
		if (image->format->Rmask == 0xFF) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
		}
		else if (image->format->Rmask == 0xFF0000) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
		}
		else {
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

bool Texture::Create(Renderer& renderer, int width, int height) {
	// Generate the texture
	glGenTextures(1, &textureName);

	if (!textureName) {
		printf("Error creating an empty texture of size %i by %i", width, height);
		return false;
	}

	// Setup the texture data and parameters
	glBindTexture(GL_TEXTURE_2D, textureName);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// Done
	return true;
}

bool Texture::CreateAsDepth(Renderer& renderer, int width, int height) {
	// Generate the texture
	glGenTextures(1, &textureName);

	if (!textureName) {
		printf("Error creating an empty texture of size %i by %i", width, height);
		return false;
	}

	// Setup the texture data and parameters
	glBindTexture(GL_TEXTURE_2D, textureName);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	// Done
	return true;
}

void Texture::Destroy() {
	// Cleanup the texture
	if (textureName) {
		glDeleteTextures(1, &textureName);
	}
}
