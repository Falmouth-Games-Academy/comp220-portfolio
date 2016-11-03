#pragma once
#include"ErrorMessage.h"
class Texture
{
public:
	Texture(const std::string& fileName);
	~Texture();
	ErrorMessage errorSystem;

	GLuint loadTexture(const std::string& fileName);

	void bindTexture();

	GLuint texture;
};

