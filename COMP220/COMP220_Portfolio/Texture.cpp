#include "stdafx.h"
#include "Texture.h"


Texture::Texture(const std::string& fileName)
{
	textureId = loadTexture(fileName);
	
	if (textureId == 0)
	{
		errorSystem.createErrorMessage("loadTexture failed", "Error");
	}
}

Texture::Texture()
{
}

Texture::~Texture()
{
}

GLuint Texture::loadTexture(const std::string& fileName)
{
	SDL_Surface* textureSurface = IMG_Load(fileName.c_str());

	if (textureSurface == nullptr)
	{
		errorSystem.createErrorMessage(SDL_GetError(), "IMG_Load failed");
		return 0;
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int format;
	if (textureSurface->format->BytesPerPixel == 3)
	{
		format = GL_RGB;
	}
	else if (textureSurface->format->BytesPerPixel == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		errorSystem.createErrorMessage("Invalid pixel format", ":(");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, textureSurface->w, textureSurface->h, 0, format, GL_UNSIGNED_BYTE, textureSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(textureSurface);
	return textureId;
}

void Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureId);
}