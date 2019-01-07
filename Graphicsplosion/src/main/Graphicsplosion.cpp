#include "stdafx.h"
#include "Graphicsplosion.h"
#include "Time.h"
#include "render/Model.h" // Test model load

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Matrix for shadow rendering
glm::mat4 shadowMVP;

const Rect2F Graphicsplosion::shadowMapDebugDimensions(0.8f, 0.8f, 0.2f, 0.2f);

Graphicsplosion game;

/* The source code is very well commented in the doxygen style.
All identifier names are descriptive and appropriate.
All code adheres to the to code guidelines (Unreal, Google etc).
There is no obvious duplication of code or of literal values. Most literal values are, where appropriate, easily “tinkered” outside of the source code
*/

void Graphicsplosion::Init() {
	// Create window and initialise renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	// Load the shaders into a map so they can be freed later
	shaders["fragment"] = render.LoadShaderFromSourceFile("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	shaders["vertex"] = render.LoadShaderFromSourceFile("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
	shaders["shadowFragment"] = render.LoadShaderFromSourceFile("src/shaders/fragmentShadow.glsl", GL_FRAGMENT_SHADER);
	shaders["shadowVertex"] = render.LoadShaderFromSourceFile("src/shaders/vertexShadow.glsl", GL_VERTEX_SHADER);
	shaders["depthDisplayFragment"] = render.LoadShaderFromSourceFile("src/shaders/fragmentDepthDisplay.glsl", GL_FRAGMENT_SHADER);

	// Setup the shader programs
	defaultShaderProgram.Create(render, shaders["vertex"], shaders["fragment"]);
	shadowShaderProgram.Create(render, shaders["vertex"], shaders["shadowFragment"]);
	shadowmapDebugShaderProgram.Create(render, shaders["vertex"], shaders["depthDisplayFragment"]);
	defaultVertexFormat.CreateFromStructVars(&Vertex::position, &Vertex::colour, &Vertex::normal, &Vertex::uvs, &Vertex::boneIndices, &Vertex::boneWeights);

	// Load the models
	pigeonModel.Create("Assets/Models/Pigeon.fbx");
	bunnyModel.Create("Assets/Models/Bunny.fbx");

	printf("Loading map - this may take a while... (blame assimp)\n");
	sceneModel.Create("Assets/Models/MainScene.fbx", true);

	// Load the textures
	pigeonTexture.Create(render, "Assets/Textures/PigeonDiffuse.png");
	bunnyTexture.Create(render, "Assets/Textures/BunnyDiffuse.png");
	groundTexture.Create(render, "Assets/Textures/Ground.png");
	whiteTexture.Create(render, "Assets/Textures/White.png");
	bunnyNormalMap.Create(render, "Assets/Textures/BunnyNormal.png");

	// Load the scene textures (todo: possibly move this later)
	for (const std::string& textureName : sceneModel.GetTextureNames()) {
		Texture* texture = new Texture(render, textureName.c_str());

		if (texture->IsLoaded()) {
			// Success!
			sceneModelTextures.push_back(texture);
		} else {
			// Texture failed to load; this texture index can refer to a null texture instead
			delete texture;
			sceneModelTextures.push_back(nullptr);
		}
	}

	// Create the background plane
	static Vertex backPlaneVertices[] = {
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	backPlane.Create(render, defaultVertexFormat, backPlaneVertices, sizeof(backPlaneVertices));

	// Spawn the player
	player.OnSpawn();
	player.SetPosition(glm::vec3(21.0f, -1.0f, 55.0f));

	// Spawn and initialise the pigeon and bunny actors
	Actor* pigeon = SpawnActor<Actor>();

	pigeon->SetPosition(glm::vec3(21.0f, 0.12f, 53.0f));
	pigeon->SetTexture(&pigeonTexture);
	pigeon->SetNormalMap(&bunnyNormalMap);
	pigeon->SetModel(&pigeonModel);
	pigeon->SetShaderProgram(&defaultShaderProgram);

	Actor* bunny = SpawnActor<Actor>();

	bunny->SetTexture(&bunnyTexture);
	bunny->SetPosition(glm::vec3(31.0f, 0.12f, 53.0f));
	bunny->SetScale(glm::vec3(0.15f, 0.15f, 0.15f));
	bunny->SetShaderProgram(&defaultShaderProgram);
	bunny->SetModel(&bunnyModel);
	
	// Setup the lights
	sunLight = DirectionalLight(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.9f, 0.9f, 0.8f));
	ambientLight = glm::vec3(0.1f, 0.1f, 0.1f);

	// Show the debug shadow map by default
	isShadowMapDebugEnabled = true;
}

void Graphicsplosion::Shutdown() {
	// Cleanup the models
	pigeonModel.Destroy();
	bunnyModel.Destroy();
	sceneModel.Destroy();

	// Cleanup the textures
	pigeonTexture.Destroy();
	bunnyTexture.Destroy();
	whiteTexture.Destroy();
	
	bunnyNormalMap.Destroy();

	for (Texture* tex : sceneModelTextures) {
		if (tex) {
			tex->Destroy();
			delete tex;
		}
	}

	// Cleanup the shaders
	// Shader programs
	defaultShaderProgram.Destroy();
	shadowShaderProgram.Destroy();
	shadowmapDebugShaderProgram.Destroy();

	// Shaders
	for (auto shader : shaders) {
		render.DestroyShader(shader.second);
	}

	// Clean up the renderer and other resources
	render.Shutdown();
	window.Destroy();
}

void Graphicsplosion::Update() {
	// Update the player
	player.Update(deltaTime);

	// Toggle the debug shadow map
	if (Input::IsKeyPressed(SDL_SCANCODE_H)) {
		isShadowMapDebugEnabled = !isShadowMapDebugEnabled;
	}
}

void Graphicsplosion::Render() {
	// Render the shadows to the shadow map
	RenderShadowPass();

	// Render the actual scenery
	RenderColourPass();
}

void Graphicsplosion::RenderColourPass() {
	// Render here
	render.BeginRender();

	// Use our default shader
	render.UseShaderProgram(defaultShaderProgram);

	// Draw the background plane
	defaultShaderProgram.SetUniform("matViewProj", glm::identity<glm::mat4>());
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	defaultShaderProgram.SetUniform("isShadowEnabled", 0);

	render.UseVertexBuffer(&backPlane);
	render.UseIndexBuffer(nullptr);
	render.UseTexture(&whiteTexture, &defaultShaderProgram);

	render.DrawTriangles(0, 6);

	// Set up the camera!
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;
	glm::mat4 matView = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());
	glm::mat4 matProj = glm::infinitePerspectiveRH(glm::radians(85.0f), (float)window.GetSize().x / (float)window.GetSize().y, 0.5f);
	glm::mat4 matViewProj = matProj * matView;

	defaultShaderProgram.SetUniform("matViewProj", matViewProj);

	// Setup the shadow map matrix at converted coordinates (UV space is between 0.0/1.0, not -1.0/1.0)
	glm::mat4 shadowUvCorrection(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	defaultShaderProgram.SetUniform("matShadowView", shadowUvCorrection * matShadowView);

	// Setup the lights!
	const glm::vec3 cameraPosition = playerEye;

	defaultShaderProgram.SetUniform("ambientLightColour", ambientLight);
	defaultShaderProgram.SetUniform("directionalLightColour", sunLight.GetColour());
	defaultShaderProgram.SetUniform("directionalLightDirection", sunLight.GetDirection());

	// Setup misc uniforms
	defaultShaderProgram.SetUniform("cameraPosition", cameraPosition);
	defaultShaderProgram.SetUniform("textureSampler", 0);
	defaultShaderProgram.SetUniform("isShadowEnabled", 1);

	// Draw every object
	for (Actor* actor : actors) {
		actor->Render(&render);
	}

	// Draw the scene
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	sceneModel.Render(render, defaultShaderProgram, sceneModelTextures.size() > 0 ? &sceneModelTextures[0] : nullptr);

	// Draw the debug shadow map onto the screen in the top-right corner
	if (isShadowMapDebugEnabled) {
		glm::mat4 shadowMapDebugTransform(
			shadowMapDebugDimensions.width, 0.0f, 0.0f, 0.0f,
			0.0f, shadowMapDebugDimensions.height, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			shadowMapDebugDimensions.x, shadowMapDebugDimensions.y, 0.0f, 1.0f
		);

		render.UseShaderProgram(shadowmapDebugShaderProgram);
		shadowmapDebugShaderProgram.SetUniform("matWorld", shadowMapDebugTransform);
		shadowmapDebugShaderProgram.SetUniform("matViewProj", glm::identity<glm::mat4>());

		render.UseTexture(render.GetShadowMap(), &shadowmapDebugShaderProgram);
		render.UseVertexBuffer(&backPlane);

		render.DrawTriangles(0, 6);
	}

	// Done!
	render.EndRender(window);
}

void Graphicsplosion::RenderShadowPass() {
	// Render here
	render.BeginRender(true, RenderPass::Shadow);

	// Use our default shader
	render.UseShaderProgram(shadowShaderProgram);

	// Setup the shadow map
	const float shadowMapRange = 25.0f;
	const float shadowDepthRange = 50.0f;

	// Setup shadow map axes and coordinates
	glm::vec3 shadowMapX = glm::cross(player.GetForward(), sunLight.GetDirection()), shadowMapY = -glm::cross(shadowMapX, sunLight.GetDirection()), shadowMapZ = sunLight.GetDirection();
	glm::vec3 shadowMapO = player.GetPosition() + shadowMapY * shadowMapRange; // make the base of the shadow map roughly the camera position

	// Generate the shadow map matrix for this frame
	matShadowView = glm::ortho(-shadowMapRange, shadowMapRange, -shadowMapRange, shadowMapRange, -shadowDepthRange, shadowDepthRange) * glm::lookAt(shadowMapO, shadowMapO + shadowMapZ, shadowMapY);

	// Shadow map is ready!
	shadowShaderProgram.SetUniform("matViewProj", matShadowView);
	
	// Render every object with the shadow shader
	for (Actor* actor : actors) {
		actor->Render(&render, &shadowShaderProgram);
	}

	// Draw the scene
	shadowShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	sceneModel.Render(render, shadowShaderProgram);

	// Done!
	render.EndRender(window, RenderPass::Shadow);
}