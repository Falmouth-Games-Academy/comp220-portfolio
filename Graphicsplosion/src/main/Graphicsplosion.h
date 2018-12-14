#pragma once
#include "Game.h"
#include "render/Renderer.h"
#include "render/Model.h"
#include "render/Light.h"

class Graphicsplosion : public Game {
public:
	Graphicsplosion() : sunLight(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(1.0f, 1.0f, -1.0f)) {}

	ShaderProgram& GetDefaultShaderProgram() {
		return defaultShaderProgram;
	}

private:
	void Init() override;
	void Shutdown() override;

	void Update() override;
	void Render() override;

	void RenderShadowPass();
	void RenderColourPass();

private:
	// Player and objects
	Player player;

	// Rendering components
	ShaderProgram defaultShaderProgram;
	ShaderProgram shadowShaderProgram;
	VertexFormat defaultVertexFormat;

	// The triangle to render
	VertexBuffer triangle;

	// The background plane for a sky effect
	VertexBuffer backPlane;
	VertexBuffer groundPlane;

	// Things to render
	Texture groundTexture;
	Texture pigeonTexture;
	Texture bunnyTexture;
	Texture whiteTexture;

	Model pigeonModel;
	Model bunnyModel;
	Model sceneModel;

	// The textures for the scene model (todo: possibly move)
	std::vector<Texture*> sceneModelTextures;

	// The light
	Light sunLight;
};

extern Graphicsplosion game; // The entire game lies here