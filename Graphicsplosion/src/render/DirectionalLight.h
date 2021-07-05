#pragma once
#include "glm/glm.hpp"

class DirectionalLight {
public:
	DirectionalLight() = default;

	// Constructs a light at the given position with the given direction
	DirectionalLight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour) {
		SetPosition(position);
		SetDirection(direction);
		SetColour(colour);
	}

public:
	// Getters and setters
	void SetPosition(const glm::vec3& position) { this->position = position; }
	const glm::vec3& GetPosition() const { return position; }

	void SetDirection(const glm::vec3& direction) { this->direction = glm::normalize(direction); }
	const glm::vec3& GetDirection() const { return direction; }

	void SetColour(const glm::vec3& colour) { this->colour = colour; }
	const glm::vec3 GetColour() const { return glm::clamp(colour, -1.0f, 1.0f); }

private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 colour;
};