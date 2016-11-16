#include "stdafx.h"
#include "ParticleEffectManager.h"


ParticleEffectManager::ParticleEffectManager()
{
	
}


ParticleEffectManager::~ParticleEffectManager()
{
}

void ParticleEffectManager::createParticles()
{
	
	for (int i = 0; i < getMaxParticlesNumber(); i++)
	{
		Particle particle(initialPosition, initialVelocity);
		auto sharedparticle = std::make_shared<Particle>(particle);
		particles.push_back(sharedparticle);
	}

}

void ParticleEffectManager::updateParticles()
{
	
}