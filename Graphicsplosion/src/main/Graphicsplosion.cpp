#include "stdafx.h"
#include "Graphicsplosion.h"
#include "Time.h"
#include "render/Model.h" // Test model load

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Graphicsplosion game;

Texture dbgTexture;
Model dbgModel;
Model dbgSecondModel;
VertexBuffer dbgModelBuffer;

// Week 9 session
/*GLuint CreateTexture(int width, int height) {
	GLuint textureId = 0;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8/*GL_RGB*//*, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	return textureId;
}

GLuint CreateDepthBuffer(int width, int height) {
	GLuint bufferId;

	glGenRenderbuffers(1, &bufferId);

	glBindRenderbuffer(GL_RENDERBUFFER, bufferId);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
}

GLuint CreateFrameBuffer(int width, int height) {
	GLuint bufferId;

	// Create and bind the frame buffer
	glGenFramebuffers(1, &bufferId);
	glBindFramebuffer(GL_RENDERBUFFER, bufferId);
	
	// Attach the frame buffer to the renderer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);

	// Attach the depth buffer to the renderer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error creating and attaching frame buffer!";
		return 0;
	}
}


glDisable(GL_DEPTH_TEST) before rendering the postprocess texture
*/

/*
In the main bit maybe bit lolasfdfsl
	GLuint colourBufferId = CreateTexture(screenWidth, screenHeight);

	Don't forget to cleanup everything!
*/

void Graphicsplosion::Init() {
	// Create window and initialise renderer
	window.Create("Zomg, it's a game! M: Unlock mouse F: Fullscreen and pay respects", Vec2I(640, 480));
	window.SetMouseLock(true);

	render.Init(window);

	dbgTexture.Create(render, "texture.jpg");

	// Load the default shaders
	GLResource fragmentShader = render.LoadShaderFromSourceFile("src/shaders/fragment.txt", GL_FRAGMENT_SHADER);
	GLResource vertexShader = render.LoadShaderFromSourceFile("src/shaders/vertex.txt", GL_VERTEX_SHADER);

	// Setup the default shader program
	defaultShaderProgram.Create(render, vertexShader, fragmentShader);

	// Generate the test triangle
	static Vertex triangleVertices[] = {
		-1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};

	// Create the test triangle
	triangle.Create(render, triangleVertices, sizeof(triangleVertices));
	
	// Load the test model
	dbgSecondModel.Create("Assets/Bunny.fbx");
	dbgModel.Create("Assets/Pigeon.fbx");

	// Create the background plane
	static Vertex backPlaneVertices[] = {
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.9999f, 0.00f, 0.64f, 0.91f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.9999f, 0.20f, 0.20f, 0.75f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 255, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	backPlane.Create(render, backPlaneVertices, sizeof(backPlaneVertices));

	// Spawn the player
	player.OnSpawn();

	// Spawn the pigeon and bunny actors
	Actor* newActor = SpawnActor<Actor>();

	newActor->SetModel(&dbgModel);
	newActor->SetModel(&dbgSecondModel);
}

void Graphicsplosion::Shutdown() {
	// Clean up the test vertex buffer
	triangle.Destroy();

	dbgTexture.Destroy();
	dbgModel.Destroy();

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

	// Use our test shader
	render.UseShaderProgram(defaultShaderProgram);

	// Test rotate the triangle
	triangleAngle += 6.28f * deltaTime * 0.25f;

	// Set up the view/proj matrix
	const float playerHeight = 0.5f;
	glm::vec3 playerEye = player.GetPosition() + player.GetUp() * playerHeight;

	glm::mat4 matViewProj = glm::lookAtRH(playerEye, playerEye + player.GetForward(), player.GetUp());
	matViewProj = glm::perspectiveFov(70.0f, (float)window.GetSize().x, (float)window.GetSize().y, 0.1f, 100.0f) * matViewProj;

	// Set up the world matrix (just a fun rotation around angle)
	glm::mat4 matWorld = glm::rotate(triangleAngle, glm::vec3(0.0f, 0.0f, 1.0f));

	// Find the uniform variables
	int uniMatWorld = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matWorld");
	int uniMatViewProj = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "matViewProj");
	int uniTime = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "time");
	int uniTexture = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "textureSampler");
	int uniAmbientLightColour = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "ambientLightColour");
	int uniDirectionalLightColour = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "directionalLightColour");
	int uniDirectionalLightDirection = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "directionalLightDirection");
	int uniViewDirection = glGetUniformLocation(defaultShaderProgram.GetGlProgram(), "uniViewDirection");

	// Upload the uniform variables
	const glm::vec3 directionalLightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	const glm::vec3 directionalLightColour = glm::vec3(0.4f, 0.4f, 0.4f);
	const glm::vec3 viewDirection = glm::normalize(glm::vec3(matViewProj[2][0], matViewProj[2][1], matViewProj[2][2]));

	const glm::vec3 ambientLight(0.25f, 0.5f, 0.5f);

	// Upload main variables that won't change this frame
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(uniMatViewProj, 1, GL_FALSE, glm::value_ptr(matViewProj));
	glUniform1f(uniTime, (float)game.frameTime);
	glUniform1i(uniTexture, 0);
	glUniform3fv(uniAmbientLightColour, 1, glm::value_ptr(ambientLight));
	glUniform3fv(uniDirectionalLightColour, 1, glm::value_ptr(directionalLightColour));
	glUniform3fv(uniDirectionalLightDirection, 1, glm::value_ptr(directionalLightDirection));
	glUniform3fv(uniViewDirection, 1, glm::value_ptr(viewDirection));

	// Set the current texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, dbgTexture.GetTextureName());
	glBindSampler(uniTexture, 1);

	// Render every object
	for (Actor* actor : actors) {
		actor->Render();
	}
	// Draw the two models
	// Draw the pigeon
	matWorld = glm::identity<glm::mat4>();
	glm::mat4 pigeonTransform = matWorld;
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(pigeonTransform));
	dbgModel.Render(render);

	// Draw the bunny
	glm::mat4 bunnyTransform = glm::scale(glm::vec3(0.15f, 0.15f, 0.15f)) * matWorld/* * glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f))*/;
	bunnyTransform = glm::translate(glm::vec3(15.0f, 0.0f, 0.0f)) * bunnyTransform;

	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(bunnyTransform));
	dbgSecondModel.Render(render);

	// Draw the background plane
	glUniformMatrix4fv(uniMatViewProj, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));
	glUniformMatrix4fv(uniMatWorld, 1, GL_FALSE, glm::value_ptr(glm::identity<glm::mat4>()));
	render.UseVertexBuffer(&backPlane);
	render.UseIndexBuffer(nullptr);

	render.DrawTriangles(0, 6);
	
	// Done!
	render.EndRender(window);
}