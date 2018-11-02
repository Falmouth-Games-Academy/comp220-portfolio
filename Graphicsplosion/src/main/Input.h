#pragma once

#include "sdl/SdlWrapper.h"

class Input {
public:
	static void Update() {
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

		// Update movement axes
		horizontalAxis = verticalAxis = 0.0f;

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
	}

public:
	// Scancode type (currently just big enough to hold an SDL_scancode)
	typedef int scancode;

	// Converts the human-readable name of a key to a scancode
	static scancode KeyNameToCode(const char* keyName) {
		return sdl.KeyNameToCode(keyName);
	}

public:
	static bool IsKeyDown(scancode key) {
		return keyStates[key] & KEYSTATE_DOWN;
	}

	static bool IsKeyPressed(scancode key) {
		return keyStates[key] & KEYSTATE_PRESSED;
	}

public:
	// Simulates a keypress
	static void SimulateKeyDown(scancode key) {
		keyStates[key] |= KEYSTATE_DOWN;
	}

	static void SimulateKeyUp(scancode key) {
		keyStates[key] &= ~KEYSTATE_DOWN;
	}

	// Simulates a joystick axis movement
	static void SimulateJoyAxis(int axisId, float axisValue) {
		if (axisId >= 0 && axisId < numJoyAxes) {
			joyAxes[axisId] = axisValue;
		}
	}

	// Simulates a joystick button press
	static void SimulateJoyDown(int buttonId) {
	}

public:
	// Returns a movement axis
	static float GetHorizontalAxis() {
		return horizontalAxis;
	}

	static float GetVerticalAxis() {
		return verticalAxis;
	}

	// Returns the mouse offset since the last frame in pixels
	static const Vec2I& GetMouseMotion() {
		return sdl.GetMouseMotion();
	}

private:
	// Keystate flag collection
	typedef unsigned char keystateflags;

	static const keystateflags KEYSTATE_DOWN     = 1; // key is down
	static const keystateflags KEYSTATE_PRESSED  = 2; // key was pressed in this frame
	static const keystateflags KEYSTATE_RELEASED = 4; // key was released in this frame
	
	// Array of current keystates
	static const int numKeyStates = 512;
	static keystateflags keyStates[numKeyStates];

	// Array of keystates from the last frame
	static keystateflags previousKeyStates[numKeyStates];

	// Array of joystick axis states
	static const int numJoyAxes = 16;
	static float joyAxes[numJoyAxes];

private:
	static float horizontalAxis;
	static float verticalAxis;
};