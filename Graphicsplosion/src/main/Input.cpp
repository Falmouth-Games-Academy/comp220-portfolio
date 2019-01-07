#include "stdafx.h"
#include "Input.h"

float Input::horizontalAxis = 0.0f;
float Input::verticalAxis = 0.0f;

float Input::eyeHorizontalAxis = 0.0f;
float Input::eyeVerticalAxis = 0.0f;

bool Input::isBoostDown = false;

Input::keystateflags Input::keyStates[numKeyStates];

// Array of keystates from the last frame
Input::keystateflags Input::previousKeyStates[numKeyStates];

float Input::joyAxes[Input::numJoyAxes];

const float Input::joystickDeadzone = 0.1f;

void Input::Update() {
	// Update keystates
	for (int key = 0; key < numKeyStates; ++key) {
		// Check if a key has been pressed since the last frame
		if ((keyStates[key] ^ previousKeyStates[key]) & (keyStates[key] & KEYSTATE_DOWN)) {
			keyStates[key] |= KEYSTATE_PRESSED;
		} else if (keyStates[key] & KEYSTATE_PRESSED) {
			keyStates[key] &= ~KEYSTATE_PRESSED;
		}

		// Check if a key has been released since the last frame
		if ((keyStates[key] ^ previousKeyStates[key]) & (previousKeyStates[key] & KEYSTATE_DOWN)) {
			keyStates[key] |= KEYSTATE_RELEASED;
		} else if (keyStates[key] & KEYSTATE_RELEASED) {
			keyStates[key] &= ~KEYSTATE_RELEASED;
		}
	}

	// Copy the current keystates to the previous keystates
	memcpy(previousKeyStates, keyStates, sizeof(keyStates));

	// Update movement axes (joystick)
	horizontalAxis = Math::ClampDeadzone(joyAxes[0], joystickDeadzone);
	verticalAxis = Math::ClampDeadzone(-joyAxes[1], joystickDeadzone);
	eyeHorizontalAxis = Math::ClampDeadzone(joyAxes[3], joystickDeadzone);
	eyeVerticalAxis = Math::ClampDeadzone(-joyAxes[4], joystickDeadzone);

	// or use key inputs for movement axes
	if (IsKeyDown(SDL_SCANCODE_W)) {
		verticalAxis += 1.0f;
	}

	if (IsKeyDown(SDL_SCANCODE_S)) {
		verticalAxis -= 1.0f;
	}

	if (IsKeyDown(SDL_SCANCODE_D)) {
		horizontalAxis += 1.0f;
	}

	if (IsKeyDown(SDL_SCANCODE_A)) {
		horizontalAxis -= 1.0f;
	}

	// Update boost key
	if (IsKeyDown(SDL_SCANCODE_LSHIFT)) {
		isBoostDown = true;
	} else {
		isBoostDown = false;
	}

	// Constrain the axes to -1, 1
	horizontalAxis = Math::ClampNormal(horizontalAxis);
	verticalAxis = Math::ClampNormal(verticalAxis);
	eyeHorizontalAxis = Math::ClampNormal(eyeHorizontalAxis);
	eyeVerticalAxis = Math::ClampNormal(eyeVerticalAxis);
}