#include "stdafx.h"
#include "Graphicsplosion.h"
#include "Time.h"
#include "render/Model.h" // Test model load

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Graphicsplosion game;

void Graphicsplosion::Init() {
	// Create window and initialise renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Create(render, vertexShader, fragmentShader);
	defaultVertexFormat.CreateFromStructVars(&Vertex::position, &Vertex::colour, &Vertex::normal, &Vertex::uvs, &Vertex::boneIndices, &Vertex::boneWeights);

	// Load the models
	pigeonModel.Create("Assets/Models/Pigeon.fbx");
	bunnyModel.Create("Assets/Models/Bunny.fbx");
	sceneModel.Create("Assets/Models/MainScene.fbx");

	// Load the textures
	pigeonTexture.Create(render, "Assets/Textures/PigeonDiffuse.png");
	bunnyTexture.Create(render, "Assets/Textures/BunnyDiffuse.png");

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

void Graphicsplosion::Render() {
	// Render here
	render.BeginRender();

	// Use our default shader
	render.UseShaderProgram(defaultShaderProgram);

	// Set up the camera!
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;

	glm::mat4 matViewProj = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());
	matViewProj = glm::perspectiveFov(70.0f, (float)window.GetSize().x, (float)window.GetSize().y, 0.1f, 100.0f) * matViewProj;

	defaultShaderProgram.SetUniform("matViewProj", matViewProj);

	// Setup the lights!
	const glm::vec3 directionalLightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	const glm::vec3 directionalLightColour = glm::vec3(0.4f, 0.4f, 0.4f);
	const glm::vec3 cameraPosition = playerEye;

	const glm::vec3 ambientLight(0.25f, 0.5f, 0.5f);

	// Setup the lights!
	//defaultShaderProgram.SetUniform("time", (float)game.frameTime);
	defaultShaderProgram.SetUniform("textureSampler", 0);
	defaultShaderProgram.SetUniform("ambientLightColour", ambientLight);
	defaultShaderProgram.SetUniform("directionalLightColour", directionalLightColour);
	defaultShaderProgram.SetUniform("directionalLightDirection", directionalLightDirection);
	defaultShaderProgram.SetUniform("cameraPosition", cameraPosition);

	// Render every object
	for (Actor* actor : actors) {
		actor->Render(&render);
	}
	
	// Draw the scene
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());
	//sceneModel.Render(render);

	// Draw the ground
	render.UseVertexBuffer(&groundPlane);

	render.DrawTriangles(0, 6);

	// Draw the background plane
	defaultShaderProgram.SetUniform("matViewProj", glm::identity<glm::mat4>());
	defaultShaderProgram.SetUniform("matWorld", glm::identity<glm::mat4>());

	render.UseVertexBuffer(&backPlane);
	render.UseIndexBuffer(nullptr);

	render.DrawTriangles(0, 6);
	
	// Done!
	render.EndRender(window);
}