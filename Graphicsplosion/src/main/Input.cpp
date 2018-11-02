#include "stdafx.h"
#include "Input.h"

float Input::horizontalAxis = 0.0f;
float Input::verticalAxis = 0.0f;

Input::keystateflags Input::keyStates[numKeyStates];

// Array of keystates from the last frame
Input::keystateflags Input::previousKeyStates[numKeyStates];

float Input::joyAxes[Input::numJoyAxes];