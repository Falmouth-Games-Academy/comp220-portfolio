#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

// An object or character with a position in space and regular updates
class Actor {
public:
	Actor(const glm::vec3& spawnPosition = glm::vec3(0.0f, 0.0f, 0.0f)) {
		position = spawnPosition;
	}

public:
	// Called upon spawn
	virtual void OnSpawn() {return;}

	// Called every frame, by default does nothing. Overridable
	virtual void Update(float deltaTime) {return;}

	// Called every frame, by default renders the model. Overridable
	virtual void Render();

public:
	// Returns / sets the actor's position
	const glm::vec3 GetPosition() const {
		return position;
	}

	void SetPosition(const glm::vec3& toPosition) {
		position = toPosition;
	}

	// Returns a local axis according to the actor's angle
	const glm::vec3 GetForward() const {
		return glm::vec3(0.0f, 1.0f, 0.0f) * angle;
	}

	const glm::vec3 GetRight() const {
		return glm::vec3(1.0f, 0.0f, 0.0f) * angle;
	}

	const glm::vec3 GetUp() const {
		return glm::vec3(0.0f, 0.0f, 1.0f) * angle;
	}

	// Sets the model to be used by this actor
	void SetModel(class Model* model_) {
		this->model = model_;
	}

protected:
	// Actor transform variables
	glm::vec3 position;
	glm::quat angle;

	// Pointer to the model we'll use for rendering
	class Model* model;
};