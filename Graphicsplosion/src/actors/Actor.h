#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

// An object or character with a position in space and regular updates
class Actor {
public:
	Actor(const glm::vec3& spawnPosition = glm::vec3(0.0f, 0.0f, 0.0f)) : model(nullptr), shader(nullptr) {
		SetPosition(spawnPosition);
		SetRotation(glm::identity<glm::quat>());
		SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	}

public:
	// Called upon spawn
	virtual void OnSpawn() {return;}

	// Called every frame, by default does nothing. Overridable
	virtual void Update(float deltaTime) {return;}

	// Called every frame, by default renders the model. Overridable
	virtual void Render(class Renderer* renderer, const class ShaderProgram* shaderOverride = nullptr) const;

public:
	// Returns / sets the actor's position
	const glm::vec3 GetPosition() const {return position;}
	void SetPosition(const glm::vec3& toPosition) {
		position = toPosition;
		transformIsDirty = true;
	}

	// Returns/sets the actors's rotation
	const glm::quat GetRotation() const { return rotation; }
	void SetRotation(const glm::quat& toRotation) {
		rotation = toRotation;
		transformIsDirty = true;
	}

	// Returns/sets the actor's scale factors
	const glm::vec3& GetScale() { return scale; }
	void SetScale(const glm::vec3 toScale) {
		scale = toScale;
		transformIsDirty = true;
	}

	// Returns a local axis according to the actor's angle
	const glm::vec3 GetForward() const {
		return glm::vec3(0.0f, 1.0f, 0.0f) * rotation;
	}

	const glm::vec3 GetRight() const {
		return glm::vec3(1.0f, 0.0f, 0.0f) * rotation;
	}

	const glm::vec3 GetUp() const {
		return glm::vec3(0.0f, 0.0f, 1.0f) * rotation;
	}

	// Returns a transform matrix representing this object's position
	const glm::mat4& GetTransform() const {
		if (transformIsDirty) {
			// Refresh the transform matrix if it's not valid
			transform = glm::identity<glm::mat4>();
			transform = glm::translate(transform, position);
			transform *= glm::toMat4(rotation);
			transform = glm::scale(transform, scale);

			// We're up-to-date!
			transformIsDirty = false;
		}

		return transform;
	}

	// Sets the model to be used by this actor when rendering
	void SetModel(class Model* model_) {
		this->model = model_;
	}

	// Sets the texture to be used by this actor when rendering. Set to nullptr for no texture.
	void SetTexture(class Texture* texture_) {
		this->texture = texture_;
	}

	// Sets the shader program to be used by this actor when rendering
	void SetShaderProgram(class ShaderProgram* shaderProgram_) {
		this->shader = shaderProgram_;
	}

protected:
	// Actor transform variables
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	mutable glm::mat4 transform;
	mutable bool transformIsDirty;

	// Pointer to the model we'll use for rendering
	class Model* model;

	// Pointer to the texture we'll use for rendering, or nullptr if none applicable
	class Texture* texture;

	// Pointer to the shader we'll use for rendering
	// matWorld will be used to position the model
	class ShaderProgram* shader;
};