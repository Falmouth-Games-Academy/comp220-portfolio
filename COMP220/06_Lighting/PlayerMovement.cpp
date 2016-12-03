#include "stdafx.h"
#include "PlayerMovement.h"

/*
having the mvp be a pointer allows for dynamic manipulation of the value without having
to set it to a new value every tick
*/

PlayerMovement::PlayerMovement(Terrain &passedTerrain)
{
	terrain = &passedTerrain;
	playerPosition = glm::vec4(0, 0, 5, 1);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

PlayerMovement::~PlayerMovement()
{
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

	glm::vec4 playerForward = playerLook;

	glm::vec4 yHeight = height(playerPosition);
	playerPosition = glm::vec4(playerPosition.x, yHeight.y, playerPosition.z, playerPosition.w);

	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	if (keyboardState[SDL_SCANCODE_W])
	{
		playerPosition += playerForward * 0.001f;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		playerPosition -= playerForward * 0.001f;
	}

	glm::vec4 playerRight(0, 0, -1, 0);
	glm::mat4 playerRightRotation;
	playerRightRotation = glm::rotate(playerRightRotation, playerYaw - glm::radians(90.0f), glm::vec3(0, 1, 0));
	playerRight = playerRightRotation * playerRight;

	if (keyboardState[SDL_SCANCODE_A])
	{
		playerPosition -= playerRight * 0.001f;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		playerPosition += playerRight * 0.001f;
	}

	glm::mat4 view = glm::lookAt(glm::vec3(playerPosition), glm::vec3(playerPosition + playerLook), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	glm::mat4 transform;
	MVP = projection * view * transform;

}

glm::mat4 PlayerMovement::getMVP()
{
	return MVP;
}

glm::vec4 PlayerMovement::height(glm::vec4 playerPosition)
{
	float nx = playerPosition.x / terrain->getXMax() * terrain->getNoiseAmp();
	float ny = playerPosition.z / terrain->getYMax() * terrain->getNoiseAmp();

	float height = terrain->getLayeredNoise(nx,ny);
	height = height * terrain->getHeightAmp();
	height += 1;

	return glm::vec4(0, height, 0, 0);
}
