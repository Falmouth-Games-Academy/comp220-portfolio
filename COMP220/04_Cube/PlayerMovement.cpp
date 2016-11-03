#include "stdafx.h"
#include "PlayerMovement.h"

/*
having the mvp be a pointer allows for dynamic manipulation of the value without having
to set it to a new value every tick
*/
PlayerMovement::PlayerMovement(glm::mat4 *mvp)
{
	MVP = mvp;
	
	glm::vec4 playerPosition(0, 0, 5, 1);
	this->playerPosition = playerPosition;
	
}


PlayerMovement::~PlayerMovement()
{
	delete MVP;
}

void PlayerMovement::tick()
{
	int mouseX, mouseY;
	SDL_GetRelativeMouseState(&mouseX, &mouseY);
	playerYaw -= mouseX * 0.005f;
	playerPitch -= mouseY * 0.005f;
	const float maxPitch = glm::radians(89.0f);
	if (playerPitch > maxPitch)
		playerPitch = maxPitch;
	if (playerPitch < -maxPitch)
		playerPitch = -maxPitch;

	glm::vec4 playerLook(0, 0, -1, 0);
	glm::mat4 playerRotation;
	playerRotation = glm::rotate(playerRotation, playerYaw, glm::vec3(0, 1, 0));
	playerRotation = glm::rotate(playerRotation, playerPitch, glm::vec3(1, 0, 0));
	playerLook = playerRotation * playerLook;

	/*glm::vec4 playerForward(0, 0, -1, 0);
	glm::mat4 playerForwardRotation;
	playerForwardRotation = glm::rotate(playerForwardRotation, playerYaw, glm::vec3(0, 1, 0));
	playerForward = playerForwardRotation * playerForward;*/
	//glm::vec4 playerForward = playerLook;

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	if (keyboardState[SDL_SCANCODE_W])
	{
		playerPosition += playerLook * movementSpeed;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		playerPosition -= playerLook * movementSpeed;
	}

	glm::vec4 playerRight(0, 0, -1, 0);
	glm::mat4 playerRightRotation;
	playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
	playerRight = playerRightRotation * playerRight;

	if (keyboardState[SDL_SCANCODE_A])
	{
		playerPosition -= playerRight * movementSpeed;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		playerPosition += playerRight * movementSpeed;
	}

	glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	glm::mat4 transform;
	//transform = glm::rotate(transform, SDL_GetTicks() / 1000.0f, glm::vec3(0, 1, 0));
	*MVP = projection * view * transform;
}
