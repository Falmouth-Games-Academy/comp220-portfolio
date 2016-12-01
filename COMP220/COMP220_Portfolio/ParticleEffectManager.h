#pragma once
#include"Particle.h"
#include"Plane.h"
//! Particle effect managing class
/*!
	Creates, updates and eliminates particles 
*/
class ParticleEffectManager
{
public:
	//! Constructor
	ParticleEffectManager(Plane* levelFloor, float intialYpos, int particleCount);
	//! Destructor
	~ParticleEffectManager();

	//! Creates particles
	void createParticle();
	//! Updates particles position
	void updateParticles(float deltaTime);
	//! Creates particle meshes
	void createMesh(const std::string& fileName);
	//! Generates coordinates for particle
	glm::vec3 generateParticlePosition();

	//! Vec3 that contains starting XYZ position of particle
	glm::vec3 initialPosition;
	//! Vec3 that contains intial velocity of particle
	glm::vec3 initialVelocity;

	//! Pointer to the plane in the main level
	/*!
	Used for phyics to make sure particles don't fall through floor
	*/
	Plane* floor;

	//! Vector containing all the current particles
	std::vector<std::shared_ptr<Particle>> particles;

	//! Returns the int maxParticleNumber
	int getMaxParticlesNumber() { return maxParticleNumber; };
	//! Texture to be used on particles
	Texture particleTexture;
	//! Particle Mesh
	Mesh particleMesh;
private: 
	//! int for the maximum number of particles that can exist at one time
	int maxParticleNumber = 20;
	//! float starting point for particle
	float intialYposition = 0;
	//! the size f the particle when rendered.
	float particleSize = 0.1;
};

