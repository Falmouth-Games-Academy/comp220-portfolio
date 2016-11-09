#pragma once
#include"ErrorMessage.h"
class Texture
{
public:
	//! Constructor
	Texture(const std::string& fileName);
	//! Constructor
	Texture();
	//! Destructor
	~Texture();

	//! Instance of error system to create errors
	ErrorMessage errorSystem;

	//! Loads texture from file
	GLuint loadTexture(const std::string& fileName);

	void bindTexture();
	GLuint loadDDS(const char * imagepath);

	GLuint texture;
	GLuint textureId;
};

