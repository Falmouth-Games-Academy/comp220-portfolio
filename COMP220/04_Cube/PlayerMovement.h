#pragma once

//this class handles player input and translates the world where necessary
class PlayerMovement
{
public:
	//initialise with a pointer to the final value to be used in render
	PlayerMovement(glm::mat4 *mvp);
	
	~PlayerMovement();




private:
	// constants for sensitivity and speed
	const float movementSpeed = 0.01;

	const float mouseSensitivity = 0.005;

	// stores the mouse position
	int mouseX, mouseY;

	glm::mat4 *mvp;
};
