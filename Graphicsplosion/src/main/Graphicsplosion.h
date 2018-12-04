#pragma once
#include "Game.h"
#include "render/Renderer.h"
#include "render/Model.h"

class Graphicsplosion : public Game {
public:
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
	VertexBuffer groundPlane;

	// Things to render
	Texture pigeonTexture;
	Texture bunnyTexture;
	Model pigeonModel;
	Model bunnyModel;
	Model sceneModel;
};

extern Graphicsplosion game; // The entire game lies here