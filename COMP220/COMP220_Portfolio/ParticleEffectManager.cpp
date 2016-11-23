#include "stdafx.h"
#include "ParticleEffectManager.h"


ParticleEffectManager::ParticleEffectManager(Floor* levelFloor)
	:floor(levelFloor)
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

void ParticleEffectManager::createParticles(const std::string& fileName)
{
	particleTexture.loadTexture(fileName);
	
	for (int i = 0; i < getMaxParticlesNumber(); i++)
	{
		Particle particle(glm::vec3(0, 5, 0), glm::vec3(0, 0, 0), floor);
		//particle.texture.
		auto sharedparticle = std::make_shared<Particle>(particle);
		particles.push_back(sharedparticle);
	}

}

void ParticleEffectManager::updateParticles(float deltaTime)
{
	for (int i = 0; i < getMaxParticlesNumber(); i++)
	{
		particles[i]->tick(deltaTime);
	}
}