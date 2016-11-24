#pragma once

#include "Terrain.h"
//this class handles player input and translates the world where necessary
class PlayerMovement
{
public:
	//initialise with a pointer to the final value to be used in render
	PlayerMovement(Terrain &passedTerrain);
	
	~PlayerMovement();

	void tick();

	glm::mat4 getMVP();

private:
	// constants for sensitivity and speed
	const float movementSpeed = 0.5;
	const float mouseSensitivity = 0.005;

	// store the players position
	glm::vec4 playerPosition;
	float playerPitch = 0;
	float playerYaw = 0;

	// stores the mouse position
	int mouseX, mouseY;

	// stores the camera position
	glm::mat4 MVP;

	//links to the terrain
	Terrain *terrain;

	//gets z height
	glm::vec4 height(glm::vec4 playerPosition);



};
