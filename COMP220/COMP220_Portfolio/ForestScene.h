#pragma once
#include"Mesh.h"

class ForestScene
{
public:
	ForestScene();
	~ForestScene();
	void showErrorMessage(const char* message, const char* title);
	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
	GLuint loadTexture(const std::string& fileName);
	void run();

private:
	SDL_Window* window;
	SDL_GLContext glContext;
	float mouseSensitivity = 0.001f;
	float movementMultipler = 0.001f;

};