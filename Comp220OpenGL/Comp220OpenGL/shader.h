#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include "transform.h"
class Shader
{
public:
	Shader(const std::string& fileName);
	
	void Bind();//set the gpu in a state so its using the shaders thats part of this parrticual class
	void Update(const Transform& transform);//update all the uniforms in our array to some new set of values
	
	virtual ~Shader();


protected:
private:
	static const unsigned int NUM_SHADER = 2;
	Shader(const Shader& other) {}
	void operator=(const Shader& other) {} //{} makes it not do anything

	//will help in future when needing to add more uniforms.(numbers to help to our uniforms)
	enum
	{
		TRANSFORM_U,


		NUM_UNIFORMS
	};


	GLuint m_program;//refers to the shader
	GLuint m_shaders[NUM_SHADER];
	GLuint m_uniforms[NUM_UNIFORMS]; //GLuint is a handle


};

#endif SHADER_H