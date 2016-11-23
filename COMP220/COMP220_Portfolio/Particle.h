#pragma once
#include"Mesh.h"
#include"OBJLoader.h"
#include"Texture.h"
#include"Floor.h"
class Particle
{
public:
	Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, Floor* levelFloor);
	~Particle();

	void tick(float deltaTime);

	float lifeSpan;
	glm::vec4 colour;

	Mesh particleMesh;
	Texture texture;
	Floor* floor;

	void addMesh();
	glm::vec3 position;
	glm::vec3 velocity;
	float size = 0.1;
	
};

