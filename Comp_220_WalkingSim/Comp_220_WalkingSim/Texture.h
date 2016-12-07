#pragma once
class Texture
{
public:
	Texture();

	GLuint loadTexture(const std::string& fileName);

	~Texture();
};

