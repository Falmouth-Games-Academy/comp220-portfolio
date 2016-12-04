#pragma once
#include "ErrorMessage.h"
class Texture
{
public:
	Texture();
	~Texture();

	GLuint loadTexture(const std::string& fileName);

private:
	ErrorMessage error;
};

