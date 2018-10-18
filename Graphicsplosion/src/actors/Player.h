#pragma once
#include "glm/gtx/quaternion.hpp"
#include "Actor.h"
#include "main/Input.h"

class Player : public Actor {
public:
	// Spawns the player at a specific position
	void OnSpawn() override;

	// Moves the player/camera
	void Update(float deltaTime) override;
};