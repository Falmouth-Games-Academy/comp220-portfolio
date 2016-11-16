#pragma once
#include"Mesh.h"
class Particle
{
public:
	Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity);
	~Particle();

	void tick(float deltaTime);

	float lifeSpan;
	glm::vec4 colour;

	Mesh particleMesh;
	GLuint texture;

private:
	glm::vec3 position;
	glm::vec3 velocity;
};

