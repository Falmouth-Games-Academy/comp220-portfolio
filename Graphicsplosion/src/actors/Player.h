#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Actor.h"
#include "main/Input.h"

class Player : public Actor {
public:
	Player(const glm::vec3& spawnPosition = glm::vec3(0.0f, 0.0f, 0.0f)) {
		position = spawnPosition;
		angle = glm::identity<glm::quat>();
	}

public:
	void Update(float deltaTime) override {
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

public:


private:
};