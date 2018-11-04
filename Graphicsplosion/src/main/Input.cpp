#include "stdafx.h"
#include "Input.h"

float Input::horizontalAxis = 0.0f;
float Input::verticalAxis = 0.0f;

float Input::eyeHorizontalAxis = 0.0f;
float Input::eyeVerticalAxis = 0.0f;

Input::keystateflags Input::keyStates[numKeyStates];

// Array of keystates from the last frame
Input::keystateflags Input::previousKeyStates[numKeyStates];

float Input::joyAxes[Input::numJoyAxes];

const float Input::joystickDeadzone = 0.1f;