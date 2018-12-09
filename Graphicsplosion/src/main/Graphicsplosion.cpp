#include "stdafx.h"
#include "Graphicsplosion.h"
#include "Time.h"
#include "render/Model.h" // Test model load

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Matrix for shadow rendering
glm::mat4 shadowMVP;

ShaderProgram debugShadowmapShader;

Graphicsplosion game;

void Graphicsplosion::Init() {
	// Create window and initialise renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);
	GLResource shadowFragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragmentShadow.txt", GL_FRAGMENT_SHADER);
	GLResource shadowVertexShader = render.LoadShaderFromSourceFile("src/shaders/vertexShadow.txt", GL_VERTEX_SHADER);
	GLResource depthTextureShader = render.LoadShaderFromSourceFile("src/shaders/fragmentDepthDebug.txt", GL_FRAGMENT_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Create(render, vertexShader, fragmentShader);
	shadowShaderProgram.Create(render, vertexShader, shadowFragmentShader);
	debugShadowmapShader.Create(render, vertexShader, depthTextureShader);
	defaultVertexFormat.CreateFromStructVars(&Vertex::position, &Vertex::colour, &Vertex::normal, &Vertex::uvs, &Vertex::boneIndices, &Vertex::boneWeights);

	// Load the models
	pigeonModel.Create("Assets/Models/Pigeon.fbx");
	bunnyModel.Create("Assets/Models/Bunny.fbx");
	sceneModel.Create("Assets/Models/MainScene.fbx");

	// Load the textures
	pigeonTexture.Create(render, "Assets/Textures/PigeonDiffuse.png");
	bunnyTexture.Create(render, "Assets/Textures/BunnyDiffuse.png");
	groundTexture.Create(render, "Assets/Textures/Ground.png");
	whiteTexture.Create(render, "Assets/Textures/White.png");

	// Create the background plane
	static Vertex backPlaneVertices[] = {
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	backPlane.Create(render, defaultVertexFormat, backPlaneVertices, sizeof(backPlaneVertices));

	// Create the ground plane
	const float groundPlaneSize = 20.0f;
	static Vertex groundPlaneVertices[] = {
		-groundPlaneSize, -groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-groundPlaneSize, -groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize, -groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	groundPlane.Create(render, defaultVertexFormat, groundPlaneVertices, sizeof(groundPlaneVertices));

	// Spawn the player
	player.OnSpawn();

	// Spawn and initialise the pigeon and bunny actors
	Actor* pigeon = SpawnActor<Actor>();

	pigeon->SetTexture(&pigeonTexture);
	pigeon->SetModel(&pigeonModel);
	pigeon->SetShaderProgram(&defaultShaderProgram);

	Actor* bunny = SpawnActor<Actor>();

	bunny->SetTexture(&bunnyTexture);
	bunny->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
	bunny->SetScale(glm::vec3(0.15f, 0.15f, 0.15f));
	bunny->SetShaderProgram(&defaultShaderProgram);
	bunny->SetModel(&bunnyModel);
}

void Graphicsplosion::Shutdown() {
	// Clean up the test vertex buffer
	triangle.Destroy();

	// Cleanup the loaded assets
	pigeonTexture.Destroy();
	bunnyTexture.Destroy();
	pigeonModel.Destroy();
	bunnyModel.Destroy();

	// Clean up the renderer and other resources
	render.Shutdown();
	window.Destroy();
}

void Graphicsplosion::Update() {
	// Update the player
	player.Update(deltaTime);
}

glm::mat4 matShadowView;

void Graphicsplosion::Render() {
	RenderShadowPass();
	RenderColourPass();
}

void Graphicsplosion::RenderColourPass() {
	// Render here
	render.BeginRender();

	// Use our default shader
	render.UseShaderProgram(defaultShaderProgram);

	// First: draw the background plane
	defaultShaderProgram.SetUniform("matViewProj", glm::identity<glm::mat4>());
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());

	render.UseVertexBuffer(&backPlane);
	render.UseIndexBuffer(nullptr);
	render.UseTexture(&whiteTexture, &defaultShaderProgram);

	render.DrawTriangles(0, 6);

	// Set up the camera!
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;
	glm::mat4 matViewProj = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());

	matViewProj = glm::perspectiveFov(70.0f, (float)window.GetSize().x, (float)window.GetSize().y, 0.1f, 100.0f) * matViewProj;

	defaultShaderProgram.SetUniform("matViewProj", matViewProj);

	// Setup the shadow map
	glm::mat4 shadowUvCorrection(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	defaultShaderProgram.SetUniform("matShadowView", shadowUvCorrection * matShadowView);

	// Setup the lights!
	const glm::vec3 directionalLightColour = glm::vec3(0.4f, 0.4f, 0.4f);
	const glm::vec3 cameraPosition = playerEye;

	const glm::vec3 ambientLight(0.25f, 0.5f, 0.5f);

	// Setup the lights!
	defaultShaderProgram.SetUniform("textureSampler", 0);
	defaultShaderProgram.SetUniform("ambientLightColour", ambientLight);
	defaultShaderProgram.SetUniform("directionalLightColour", directionalLightColour);
	defaultShaderProgram.SetUniform("directionalLightDirection", sunLight.GetDirection());
	defaultShaderProgram.SetUniform("cameraPosition", cameraPosition);

	// Render every object
	for (Actor* actor : actors) {
		actor->Render(&render);
	}

	// Draw the scene
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	//sceneModel.Render(render);

	// Draw the ground
	render.UseTexture(&groundTexture, &defaultShaderProgram);
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Draw the shadow map
	glm::mat4 shadowMapDebug(
		0.0f, 0.0f, 0.1f, 0.0f,
		0.1f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.0f, 0.0f,
		-10.0f, -6.0f, 2.5f, 1.0f
	);

	render.UseShaderProgram(debugShadowmapShader);

	debugShadowmapShader.SetUniform("matWorld", shadowMapDebug);
	debugShadowmapShader.SetUniform("matViewProj", matViewProj);

	render.UseTexture(render.GetShadowMap(), &debugShadowmapShader);
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Done!
	render.EndRender(window);
}

void Graphicsplosion::RenderShadowPass() {
	// Render here
	render.BeginRender(true, RenderPass::Shadow);

	// Use our default shader
	render.UseShaderProgram(shadowShaderProgram);

	// Setup the shadow render matrix
	matShadowView = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 50.0f) * glm::lookAt(sunLight.GetPosition(), sunLight.GetDirection(), glm::vec3(0.0f, 0.0f, 1.0f));
	shadowShaderProgram.SetUniform("matViewProj", matShadowView);
	
	// Render every object with the shadow shader
	for (Actor* actor : actors) {
		actor->Render(&render, &shadowShaderProgram);
	}

	// Draw the scene
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	//sceneModel.Render(render);

	// Draw the ground
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Done!
	render.EndRender(window, RenderPass::Shadow);
}