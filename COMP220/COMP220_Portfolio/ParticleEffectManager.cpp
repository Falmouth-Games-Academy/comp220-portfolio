#include "stdafx.h"
#include "ParticleEffectManager.h"


ParticleEffectManager::ParticleEffectManager(Plane* levelFloor, float intialYpos, int particleCount)
	:floor(levelFloor), intialYposition(intialYpos), maxParticleNumber(particleCount)
{
	createParticle();
}


ParticleEffectManager::~ParticleEffectManager()
{
}


void ParticleEffectManager::createMesh(const std::string& fileName)
{
	particleMesh.addSphere(particleSize, 1, glm::vec3(0.0, 1.0, 1.0));
	particleMesh.createBuffers();
	particleTexture.loadTexture(fileName);
}


glm::vec3 ParticleEffectManager::generateParticlePosition()
{
	// Generates random X and Z value between between -1 and 1
	float randomX = ((float)rand() / (RAND_MAX)) * 2 - 1;
	float randomZ = ((float)rand() / (RAND_MAX)) * 2 - 1;
	// Generates random Y value between 0 and 1 and divides it by 10
	float randomY = rand() % 5 + intialYposition;
	return glm::vec3(randomX, randomY, randomZ);
}

void ParticleEffectManager::createParticle()
{
	glm::vec3 particlePosition = generateParticlePosition();
	Particle particle(particlePosition, glm::vec3(0, 0, 0), floor);
	particle.lifeSpan = (rand() % 10) * 100;

	auto sharedparticle = std::make_shared<Particle>(particle);
	particles.push_back(sharedparticle);


}

void ParticleEffectManager::updateParticles(float deltaTime)
{

	while (particles.size() < getMaxParticlesNumber())
	{
		createParticle();
	}
	for (int i = 0; i < particles.size(); i++)
	{
		
		if (particles[i]->lifeSpan > 0)
		{
			particles[i]->update(deltaTime);
			particles[i]->lifeSpan -= 1;
		}
		else if (particles[i]->lifeSpan < 1)
		{
			particles.erase(particles.begin() + i);
		}
	}
}


