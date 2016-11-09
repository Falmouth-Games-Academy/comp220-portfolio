#pragma once
#include"Texture.h"
class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void createParticleBuffers();
	void drawParticles();

	void sortParticles();
	int FindUnusedParticle();
	void mainFunction();

	const int maxParticles = 1000;  //TODO :Fix

	GLuint billboardVertexBuffer;
	GLuint particlesPositionBuffer;
	GLuint particlesColorBuffer;

	Texture particleTexture;

	int LastUsedParticle = 0;
	int particlesCount = 0;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
};

