#pragma once


//handles both the loading of shader but also the compiling of shaders 
class LoadShader
{
public:
	LoadShader();
	~LoadShader();

	//loads the shader
	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);

private:

	//compiles the shader
	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
};

