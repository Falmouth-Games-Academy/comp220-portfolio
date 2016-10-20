#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader
{
public:
	Shader(const std::string& fileName);
	
	void Bind();//set the gpu in a state so its using the shaders thats part of this parrticual class
	
	
	virtual ~Shader();


protected:
private:
	static const unsigned int NUM_SHADER = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {} //{} makes it not do anything

	GLuint m_program;//refers to the shader
	GLuint m_shaders[NUM_SHADER];


};

#endif SHADER_H