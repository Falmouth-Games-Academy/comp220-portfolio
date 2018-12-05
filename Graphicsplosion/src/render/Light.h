#pragma once
#include "glm/glm.hpp"

class Light {
public:
	Light(const glm::vec3& position, const glm::vec3& direction) {
		SetPosition(position);
		SetDirection(direction);
	}

public:
	// Gets and sets position and direction of the light
	void SetPosition(const glm::vec3& position) { this->position = position; }
	const glm::vec3 GetPosition() const { return position; }

	void SetDirection(const glm::vec3& direction) { this->direction = glm::normalize(direction); }
	const glm::vec3 GetDirection() const { return direction; }

private:
	glm::vec3 position;
	glm::vec3 direction;
};