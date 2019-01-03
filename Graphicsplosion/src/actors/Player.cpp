#include "stdafx.h"
#include "Player.h"

void Player::OnSpawn() {
	position = glm::vec3(0.0f, -1.0f, 0.0f);
	rotation = glm::identity<glm::quat>();
}

void Player::Update(float deltaTime) {
	const float playerSpeed = 15.0f; // in units/sec
	const float joystickCameraSpeed = 6.28; // in radians/sec
	float boostSpeed = Input::IsBoostDown() ? 2.0f : 1.0f; // 2x speed when Shift is held

	// Move
	position += GetForward() * (deltaTime * Input::GetVerticalAxis() * playerSpeed * boostSpeed);
	position += GetRight() * (deltaTime * Input::GetHorizontalAxis() * playerSpeed * boostSpeed);

	// Rotate by mouse
	rotation = rotation * glm::quat(glm::vec3(0.0f, 0.0f, 0.005f * Input::GetMouseMotion().x));
	rotation = rotation * glm::angleAxis(0.005f * Input::GetMouseMotion().y, GetRight());

	// Rotate by joystick if applicable
	rotation = rotation * glm::quat(glm::vec3(0.0f, 0.0f, joystickCameraSpeed * Input::GetEyeHorizontalAxis() * deltaTime));
	rotation = rotation * glm::angleAxis(joystickCameraSpeed * Input::GetEyeVerticalAxis() * deltaTime, GetRight());

	// Normalise rotation in case of inaccuracies
	glm::normalize(rotation);
}