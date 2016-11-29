#include "stdafx.h"
#include "ParticleEffectManager.h"


ParticleEffectManager::ParticleEffectManager(Plane* levelFloor, float intialYpos, int particleCount)
	:floor(levelFloor), intialYposition(intialYpos), maxParticleNumber(particleCount)
{
	
}


ParticleEffectManager::~ParticleEffectManager()
{
}


void ParticleEffectManager::createMesh(Particle particle)
{
	particle.particleMesh.addSphere(particle.size, 1, glm::vec3(0.0, 1.0, 1.0));
	particle.particleMesh.createBuffers();
	particle.texture;
}

void ParticleEffectManager::createParticle(const std::string& fileName)
{
	particleMesh.addSphere(particleSize, 1, glm::vec3(0.0, 1.0, 1.0));
	particleMesh.createBuffers();
	particleTexture.loadTexture(fileName);
	
	for (int i = 0; i < getMaxParticlesNumber(); i++)
	{
		// Generates random X and Z value between between -1 and 1
		float randomX = ((float)rand() / (RAND_MAX)) *2 - 1;
		float randomZ = ((float)rand() / (RAND_MAX)) * 2 - 1;
		// Generates random Y value between 0 and 1 and divides it by 10
		float randomY = ((float)rand() / (RAND_MAX)) / 10;


		//Creates particles
		Particle particle(glm::vec3(randomX, intialYposition + randomY, randomZ), glm::vec3(0, 0, 0), floor);
		particle.lifeSpan = ((float)rand() / (RAND_MAX)) * 10;
		auto sharedparticle = std::make_shared<Particle>(particle);
		particles.push_back(sharedparticle);
	}

}

void ParticleEffectManager::updateParticles(float deltaTime)
{
	for (int i = 0; i < getMaxParticlesNumber(); i++)
	{
		particles[i]->update(deltaTime);
	}
}


