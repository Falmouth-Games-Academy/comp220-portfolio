#pragma once
#include"Mesh.h"
#include"ErrorMessage.h"
#include"Texture.h"
#include"OBJLoader.h"
#include"ParticleEffectManager.h"
#include"Shader.h"

class ForestScene
{
public:
	//! Constructor
	ForestScene();
	// Destructor
	~ForestScene();

	void run();

	ErrorMessage errorSystem;
	Shader shaders;

private:
	SDL_Window* window;
	SDL_GLContext glContext;

	float mouseSensitivity = 0.01f;
	float movementMultipler = 0.01f;

	float playerPitch = 0;
	float playerYaw = 0;
};