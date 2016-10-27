#pragma once

//this class handles player input and translates the world where necessary
class PlayerMovement
{
public:
	PlayerMovement();
	~PlayerMovement();


private:
	const float movementSpeed = 0.01;

	const float mouseSensitivity = 0.005;
};
