#pragma once
#include"Particle.h"
#include"Floor.h"
class ParticleEffectManager
{
public:
	//! Constructor
	ParticleEffectManager(Floor* levelFloor);
	//! Destructor
	~ParticleEffectManager();

	void createParticles(const std::string& fileName);
	void updateParticles(float deltaTime);
	void createMesh(Particle particle);

	// starting point for all particles 
	glm::vec3 initialPosition;
	glm::vec3 initialVelocity;

	Floor* floor;

	std::vector<std::shared_ptr<Particle>> particles;

	int getMaxParticlesNumber() { return maxParticleNumber; };
	Texture particleTexture;
private: 
	int maxParticleNumber = 20;
};

