#pragma once
#include"ErrorMessage.h"
//! Texture classs
/*!
	Loads and binds textures from image files
*/
class Texture
{
public:
	//! Constructor
	Texture();
	//! Alternate constructor that takes texture file location
	Texture(const std::string& fileName);
	//! Destructor
	~Texture();

	//! Instance of error system to create errors
	ErrorMessage errorSystem;

	//! Loads texture from file
	GLuint loadTexture(const std::string& fileName);

	//! Binds the texture which needs to be active
	void bindTexture();

	//! GLuint that stores textureID
	GLuint textureId;
};

