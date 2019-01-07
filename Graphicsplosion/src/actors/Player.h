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

private:
	// Max player speed in units/sec
	const float defaultSpeed = 15.0f;
	const float runSpeed = 30.0f;

	// Speed of the camera when controlled by a joystick in radians/sec
	const float joystickCameraSpeed = 6.28;
};