#include "stdafx.h"
#include "Particle.h"


Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, Floor* levelFloor)
	: position(initialPosition), velocity(initialVelocity), floor(levelFloor)
{
	texture.loadTexture("Textures/leaf.png");
}


Particle::~Particle()
{
}

void Particle::addMesh()
{

}

void Particle::tick(float deltaTime)
{
	position += deltaTime * velocity;

	// Apply collision response
	if (position.y - size < floor->getY() && velocity.y < 0)
	{
		velocity.y *= -0.4f;
	}

	// Apply gravity
	glm::vec3 acceleration(0, -9.81f, 0);

	// Update velocity
	velocity += deltaTime * acceleration;
}
