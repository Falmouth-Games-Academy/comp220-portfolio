#pragma once
#include"Mesh.h"
#include"OBJLoader.h"
#include"Texture.h"
#include"Plane.h"
//! Particle class
class Particle
{
public:
	//! Constructor
	Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, Plane* levelFloor);
	//! Destructor
	~Particle();
	
	//! Updates the particle position
	void update(float deltaTime);

	//! float for how long the particle should be rendered on screen
	float lifeSpan;
	//! vec4 of the particles colour when rendered
	glm::vec4 colour;
	
	//! Particle mesh
	Mesh particleMesh;
	//! Particle texture
	Texture texture;
	//! Pointer to the level floor for use in physics
	Plane* floor;

	//! Adds a mesh to the particle
	void addMesh();
	//! vec3 of the particle position
	glm::vec3 position;
	//! vec3 of the particle velocity
	glm::vec3 velocity;
	//! float for the size of the particle when rendered
	float size = 0.1;
	
};

