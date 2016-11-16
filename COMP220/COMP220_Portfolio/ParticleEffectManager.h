#pragma once
#include"Particle.h"
class ParticleEffectManager
{
public:
	//! Constructor
	ParticleEffectManager();
	//! Destructor
	~ParticleEffectManager();

	void createParticles();
	void updateParticles();

	// starting point for all particles 
	glm::vec3 initialPosition;
	glm::vec3 initialVelocity;

	std::vector<std::shared_ptr<Particle>> particles;

	int getMaxParticlesNumber() { return maxParticleNumber; };

private: 
	int maxParticleNumber = 20;
};

