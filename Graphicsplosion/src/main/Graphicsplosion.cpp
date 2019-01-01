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

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.glsl", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.glsl", GL_VERTEX_SHADER);
	GLResource shadowFragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragmentShadow.glsl", GL_FRAGMENT_SHADER);
	GLResource shadowVertexShader = render.LoadShaderFromSourceFile("src/shaders/vertexShadow.glsl", GL_VERTEX_SHADER);
	GLResource depthTextureShader = render.LoadShaderFromSourceFile("src/shaders/fragmentDepthDebug.glsl", GL_FRAGMENT_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Create(render, vertexShader, fragmentShader);
	shadowShaderProgram.Create(render, vertexShader, shadowFragmentShader);
	debugShadowmapShader.Create(render, vertexShader, depthTextureShader);
	defaultVertexFormat.CreateFromStructVars(&Vertex::position, &Vertex::colour, &Vertex::normal, &Vertex::uvs, &Vertex::boneIndices, &Vertex::boneWeights);

	// Load the models
	pigeonModel.Create("Assets/Models/Pigeon.fbx");
	bunnyModel.Create("Assets/Models/Bunny.fbx");

	printf("Loading map - this may take a while... (blame assimp)\n");
	//sceneModel.Create("Assets/Models/MainScene.fbx", true);

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
		-groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-groundPlaneSize, -groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize,  groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 groundPlaneSize, -groundPlaneSize, 0.0f, 0.09f, 0.7f, 0.75f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	groundPlane.Create(render, defaultVertexFormat, groundPlaneVertices, sizeof(groundPlaneVertices));

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

	// Clean up the renderer and other resources
	render.Shutdown();
	window.Destroy();
}

void Graphicsplosion::Update() {
	// Update the player
	player.Update(deltaTime);
}

void Graphicsplosion::Render() {
	RenderShadowPass();
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
	glm::mat4 matProj = glm::infinitePerspectiveRH(70.0f, (float)window.GetSize().x / (float)window.GetSize().y, 0.5f);
	glm::mat4 matOrtho = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f);
	glm::mat4 matViewProj = matProj * matView;
	glm::mat4 matMovement = glm::translate(glm::vec3(10.0f, 10.0f, 10.0f));

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
	const glm::vec3 directionalLightColour = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 cameraPosition = playerEye;

	const glm::vec3 ambientLight(0.5f, 0.5f, 0.5f);

	// Setup the lights!
	defaultShaderProgram.SetUniform("textureSampler", 0);
	defaultShaderProgram.SetUniform("ambientLightColour", ambientLight);
	defaultShaderProgram.SetUniform("directionalLightColour", directionalLightColour);
	defaultShaderProgram.SetUniform("directionalLightDirection", sunLight.GetDirection());
	defaultShaderProgram.SetUniform("cameraPosition", cameraPosition);
	defaultShaderProgram.SetUniform("isShadowEnabled", 1);

	// Render every object
	for (Actor* actor : actors) {
		actor->Render(&render);
	}

	// Draw the scene
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	sceneModel.Render(render, defaultShaderProgram, sceneModelTextures.size() > 0 ? &sceneModelTextures[0] : nullptr);

	// Draw the ground
	render.UseTexture(&groundTexture, &defaultShaderProgram);
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Draw the test shadow map plane
	glm::mat4 shadowMapDebug(
		0.2f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.2f, 0.0f, 0.0f,
		-10.0f, -6.0f, 55.0f, 1.0f
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
	const float shadowMapRange = 50.0f;
	const float shadowDepthRange = 50.0f;
	matShadowView = glm::ortho(-shadowMapRange, shadowMapRange, -shadowMapRange, shadowMapRange, -shadowDepthRange, shadowDepthRange) 
				  * glm::lookAt(player.GetPosition(), player.GetPosition() + /*player.GetForward()*/sunLight.GetDirection(), player.GetUp());

	// Experiment: Use w division to reduce shadow precision towards the distance
	glm::mat4 shadowPrecisionDivider = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 2.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	matShadowView = shadowPrecisionDivider * matShadowView;

	shadowShaderProgram.SetUniform("matViewProj", matShadowView);
	
	// Render every object with the shadow shader
	for (Actor* actor : actors) {
		actor->Render(&render, &shadowShaderProgram);
	}

	// Draw the scene
	shadowShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	sceneModel.Render(render, shadowShaderProgram);

	// Draw the ground
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Done!
	render.EndRender(window, RenderPass::Shadow);
}