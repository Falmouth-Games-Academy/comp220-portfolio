#include "stdafx.h"
#include "Player.h"

void Player::OnSpawn() {
	position = glm::vec3(0.0f, -1.0f, 0.0f);
	angle = glm::identity<glm::quat>();
}

void Player::Update(float deltaTime) {
	const float playerSpeed = 3.0f;

	// Move
	position += GetForward() * (deltaTime * Input::GetVerticalAxis() * playerSpeed);
	position += GetRight() * (deltaTime * Input::GetHorizontalAxis() * playerSpeed);

	// Rotate by mouse
	angle = angle * glm::quat(glm::vec3(0.0f, 0.0f, 0.01f * Input::GetMouseMotion().x));
	angle = angle * glm::angleAxis(0.01f * Input::GetMouseMotion().y, GetRight());

	// I guess?
	glm::normalize(angle);
}