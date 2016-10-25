#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
Texture::Texture(const std::string& fileName)
{
	
	int width, height, numComponents;
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);		//load texture data

	if (imageData == NULL)
	{
		std::cerr << "Texture loading fail texture (Texture.cpp) " << fileName << std::endl;
	}

	glGenTextures(1, &m_texture); //generate 1 space texture and give handle in m_texture
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//controls texture wraping, reading outside the texture width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//controls texture wraping, reading outside the texture height
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//when the texture takes up less pixels
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//when the texture takes up more pixels
	
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA, GL_UNSIGNED_BYTE,imageData);

	stbi_image_free(imageData); //delete texture data from cpu when not needed

}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);//deletes the texture


}


void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);	//stops the units to go out of the range (invalid texture unit)

	glActiveTexture(GL_TEXTURE0 + unit);//set active to whatever we specify  then bind it to the unit
	glBindTexture(GL_TEXTURE_2D, m_texture);//all future texture poerations will be used texture stored here
}