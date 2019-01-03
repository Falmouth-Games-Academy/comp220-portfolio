#pragma once

#include "helpers/math.h"
#include "sdl/SdlWrapper.h"

class Input {
public:
	// Updates user inputs and key state lists
	static void Update();

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
			joyAxes[axisId] = axisValue / 32767.0f;
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

	static float GetEyeHorizontalAxis() {
		return eyeHorizontalAxis;
	}

	static float GetEyeVerticalAxis() {
		return eyeVerticalAxis;
	}

	// Returns whether the speed boost button is held
	static bool IsBoostDown() {
		return isBoostDown;
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

	static float eyeHorizontalAxis;
	static float eyeVerticalAxis;

	static bool isBoostDown;

private:
	static const float joystickDeadzone;
};