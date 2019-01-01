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

private:
	// Renders the world to the shadow map
	void RenderShadowPass();

	// Renders the world to the screen
	void RenderColourPass();

private:
	// Player and objects
	Player player;

	// Rendering components
	ShaderProgram defaultShaderProgram;
	ShaderProgram shadowShaderProgram;
	VertexFormat defaultVertexFormat;

	// The background plane for a sky effect
	VertexBuffer backPlane;
	VertexBuffer groundPlane;

	// Texture assets to render
	Texture groundTexture;
	Texture pigeonTexture;
	Texture whiteTexture;

	Texture bunnyTexture;
	Texture bunnyNormalMap;

	// Model assets to render
	Model pigeonModel;
	Model bunnyModel;
	Model sceneModel;

	// The textures for the scene model (todo: possibly move)
	std::vector<Texture*> sceneModelTextures;

	// The world light
	Light sunLight;

	// The shadow map matrix, used to transform to shadow map space
	glm::mat4 matShadowView;
};

extern Graphicsplosion game; // The entire game lies here