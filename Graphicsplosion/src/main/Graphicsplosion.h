#pragma once
#include "Game.h"
#include "render/Renderer.h"
#include "render/Model.h"
#include "render/DirectionalLight.h"
#include "helpers/math.h"

class Graphicsplosion : public Game {
private:
	// Load and initialise Graphicsplosion
	void Init() override;

	// Shuts down and frees all resources
	void Shutdown() override;

	// Do per-frame game update
	void Update() override;

	// Do per-frame game render
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
	ShaderProgram shadowmapDebugShaderProgram;
	VertexFormat defaultVertexFormat;

	// The background plane for a sky effect
	VertexBuffer backPlane;

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

	// Shader resources
	std::map<std::string, GLResource> shaders;

	// The world lights
	DirectionalLight sunLight;
	glm::vec3 ambientLight;

	// The shadow map matrix, used to transform to shadow map space
	glm::mat4 matShadowView;

	// Dimensions to render the debug shadow map
	static const Rect2F shadowMapDebugDimensions;
	bool isShadowMapDebugEnabled;
};

extern Graphicsplosion game; // The entire game lies here