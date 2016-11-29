#pragma once
#include"ErrorMessage.h"
//! Shader class
/*!
	Loads and compliles fragment and vertex shaders from files
*/
class Shader
{
public:
	//! Constructor
	Shader();
	//! Destructor
	~Shader();

	//! Error system to create error windows
	ErrorMessage errorSystem;

	//! Loads shader file and returns a GLuint
	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
	//! Compliles shader and returns a bool
	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
};

