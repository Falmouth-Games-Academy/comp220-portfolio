#include "stdafx.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
	if (billboardVertexBuffer != 0)
		glDeleteBuffers(1, &billboardVertexBuffer);

	if (particlesPositionBuffer != 0)
		glDeleteBuffers(1, &particlesPositionBuffer);

	if (particlesColorBuffer != 0)
		glDeleteBuffers(1, &particlesColorBuffer);
}


struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameraDistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameraDistance > that.cameraDistance;
	}
};

Particle ParticlesContainer[100];

void ParticleSystem::createParticleBuffers()
{
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};
	
	glGenBuffers(1, &billboardVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboardVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &particlesPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlesPositionBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	glGenBuffers(1, &particlesColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, particlesColorBuffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void ParticleSystem::drawParticles()
{
	glBindBuffer(GL_ARRAY_BUFFER, particlesPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particlesColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(GLubyte) * 4, g_particule_color_data);
}

void ParticleSystem::sortParticles() {
	std::sort(&ParticlesContainer[0], &ParticlesContainer[1000]);
}

// Finds a Particle in ParticlesContainer which isn't used yet.
int ParticleSystem::FindUnusedParticle() {

	for (int i = LastUsedParticle; i<maxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleSystem::mainFunction()
{
	g_particule_position_size_data = new GLfloat[maxParticles * 4];
	g_particule_color_data = new GLubyte[maxParticles * 4];

	for (int i = 0; i<maxParticles; i++) {
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameraDistance = -1.0f;
	}

	GLuint Texture = particleTexture.loadDDS("particle.DDS");

	createParticleBuffers();
	double lastTime = SDL_GetTicks() / 1000;
}

