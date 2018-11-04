#include "stdafx.h"
#include "Player.h"

void Player::OnSpawn() {
	position = glm::vec3(0.0f, -1.0f, 0.0f);
	angle = glm::identity<glm::quat>();
}

void Player::Update(float deltaTime) {
	const float playerSpeed = 15.0f; // in units/sec
	const float joystickCameraSpeed = 6.28; // in radians/sec

	// Move
	position += GetForward() * (deltaTime * Input::GetVerticalAxis() * playerSpeed);
	position += GetRight() * (deltaTime * Input::GetHorizontalAxis() * playerSpeed);

	// Rotate by mouse
	angle = angle * glm::quat(glm::vec3(0.0f, 0.0f, 0.005f * Input::GetMouseMotion().x));
	angle = angle * glm::angleAxis(0.005f * Input::GetMouseMotion().y, GetRight());

	// Rotate by joystick if applicable
	angle = angle * glm::quat(glm::vec3(0.0f, 0.0f, joystickCameraSpeed * Input::GetEyeHorizontalAxis() * deltaTime));
	angle = angle * glm::angleAxis(joystickCameraSpeed * Input::GetEyeVerticalAxis() * deltaTime, GetRight());

	// I guess?
	glm::normalize(angle);
}