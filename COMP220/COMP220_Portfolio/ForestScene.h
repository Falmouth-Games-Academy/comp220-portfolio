#pragma once
#include"Mesh.h"
#include"ErrorMessage.h"
#include"Texture.h"
#include"OBJLoader.h"
#include"ParticleEffectManager.h"
#include"Shader.h"
#include"Plane.h"
//! Main clas where everything runs
class ForestScene
{
public:
	//! Constructor
	ForestScene();
	// Destructor
	~ForestScene();

	//! Run function for while the game is running
	/*!
		Creates instances of objects, updates them and renders them
	*/
	void run();

	//! Error system to create error messges
	ErrorMessage errorSystem;
	//! Shader class used to load and compile shaders
	Shader shaders;
	//! OBJLoader used to load OBJ files into meshes
	OBJLoader treeModel;
	//! Loads tree model and texures
	void loadTreeModel();

private:
	//! Pointer to SDL_Window 
	SDL_Window* window;
	//! Pointer to SDL_GLConetext
	SDL_GLContext glContext;

	//! Float for mouse movement 
	float mouseSensitivity = 0.01f;
	//! Float for player/keyboard movement
	float movementMultipler = 0.05f;

	//! Float for playerPitch
	float playerPitch = 0;
	//! Float for playerYaw
	float playerYaw = 0;
	//! ints for mouse X and Y coordinates
	int mouseX, mouseY;

	const Uint32 timePerUpdate = 1000 / 60;
};