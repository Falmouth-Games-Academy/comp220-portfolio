#pragma once
#include "Game.h"

class Graphicsplosion : public Game {
public:
	Graphicsplosion() : triangleAngle(0.0f) {}

	ShaderProgram& GetDefaultShaderProgram() {
		return defaultShaderProgram;
	}

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
	VertexFormat defaultVertexFormat;

	// The triangle to render
	VertexBuffer triangle;

	// The background plane for a sky effect
	VertexBuffer backPlane;

	// Current rotation angle of the triangle
	float triangleAngle;
};

extern Graphicsplosion game; // The entire game lies here