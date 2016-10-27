#pragma once
#include"Mesh.h"

class ForestScene
{
public:
	//! Constructor
	ForestScene();
	// Destructor
	~ForestScene();

	//! Intialsises lighting
	void initLighting();
	//! Creates error pop up for errors on Windows
	void showErrorMessage(const char* message, const char* title);
	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
	GLuint loadTexture(const std::string& fileName);
	void run();
	glm::vec4 calculateCameraAngle(int mouseX, int mouseY);


private:
	SDL_Window* window;
	SDL_GLContext glContext;
	float mouseSensitivity = 0.01f;
	float movementMultipler = 0.01f;

	
	float playerPitch = 0;
	float playerYaw = 0;


};