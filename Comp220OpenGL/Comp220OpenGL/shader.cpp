#include "shader.h"
#include <iostream>
#include <fstream>
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	
	m_program = glCreateProgram(); //creating a new shader program
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName+ ".fs"), GL_FRAGMENT_SHADER);//.vs vertex shader, .fs fragment shader

	for (unsigned int i = 0; i < NUM_SHADER; i++) {//add shader to program
		glAttachShader(m_program, m_shaders[i]);
	}


	glBindAttribLocation(m_program, 0, "position");//it tells opengl what part of the data to read in what variable in the shader 
	glBindAttribLocation(m_program, 1, "texCoord");//find attribute texCoord and bind it to 1
	
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "program linking failure!!!:  ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "program validation failure:  ");//after error checks checks to see if the program is still valid
	
}


Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER; i++)
	{
		glDetachShader(m_program, m_shaders[i]);//removes eversingle shader
		glDeleteShader(m_shaders[i]);			//Deletes everysingle shader
	}
	
	glDeleteProgram(m_program);					//deletes program
}

void Shader::Bind()
{
	glUseProgram(m_program);
}




static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0) {
		std::cerr << "Shader creation Fail" << std::endl;
	}
	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];
	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "shader compile failure:  ");
	return shader;
}

static std::string LoadShader(const std::string& fileName) //Loads a file off the hardrive
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		std::cerr << "unable to load shader: " << fileName << std::endl;
	}
	return output;


}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)//checks for shader errors
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cerr << errorMessage << ":  '" << error << "' " << std::endl;
	}

}
