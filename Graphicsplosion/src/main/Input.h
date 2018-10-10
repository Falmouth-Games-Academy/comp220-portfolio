#pragma once

#include "sdl/SdlWrapper.h"

class Input {
public:
	static void Update() {
		// Update movement axes
		horizontalAxis = verticalAxis = 0.0f;

		if (sdl.CheckKeyDown("W")) {
			verticalAxis += 1.0f;
		}

		if (sdl.CheckKeyDown("S")) {
			verticalAxis -= 1.0f;
		}

		if (sdl.CheckKeyDown("D")) {
			horizontalAxis += 1.0f;
		}

		if (sdl.CheckKeyDown("A")) {
			horizontalAxis -= 1.0f;
		}
	}

	// Returns a movement axis
	static float GetHorizontalAxis() {
		return horizontalAxis;
	}

	static float GetVerticalAxis() {
		return verticalAxis;
	}

	static const Vec2I& GetMouseMotion() {
		return sdl.GetMouseMotion();
	}

private:
	static float horizontalAxis;
	static float verticalAxis;
};