#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
class Texture
{

public:
	Texture(const std::string& fileName);

	void Bind(unsigned int unit); //Unsigned because you can bind multiple textures at once(31)



	virtual ~Texture();


protected:
private:
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}

	GLuint m_texture;  //somewhere to store the handle for the texture
};

#endif TEXTURE_H