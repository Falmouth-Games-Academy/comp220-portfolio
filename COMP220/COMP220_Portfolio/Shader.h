#pragma once
#include"ErrorMessage.h"
class Shader
{
public:
	Shader();
	~Shader();

	ErrorMessage errorSystem;

	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
};

