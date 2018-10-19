#pragma once
#include "Game.h"

class Graphicsplosion : public Game {
public:
	Graphicsplosion() : triangleAngle(0.0f) {}

private:
	void Init() override;
	void Shutdown() override;

	void Update() override;
	void Render() override;

private:
	// Player and objects
	Player player;

	// Rendering components
	ShaderProgram defaultShaderProgram;

	// The triangle to render
	VertexBuffer triangle;

	// Current rotation angle of the triangle
	float triangleAngle;
};

extern Graphicsplosion game; // The entire game lies here