#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	GLuint loadTexture(const std::string& fileName);
};

