#pragma once
#include"Mesh.h"
#include"ErrorMessage.h"
#include"Texture.h"
#include"OBJLoader.h"
#include"ParticleEffectManager.h"

class ForestScene
{
public:
	//! Constructor
	ForestScene();
	// Destructor
	~ForestScene();

	bool compileShader(GLuint shaderId, const std::string& shaderFileName);
	GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
	void run();

	ErrorMessage errorSystem;


private:
	SDL_Window* window;
	SDL_GLContext glContext;

	float mouseSensitivity = 0.01f;
	float movementMultipler = 0.01f;

	float playerPitch = 0;
	float playerYaw = 0;
};