#include "stdafx.h"
#include "Particle.h"


Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
	: position(initialPosition), velocity(initialVelocity)
{
}


Particle::~Particle()
{
}


void Particle::tick(float deltaTime)
{
	// Update position
	position += deltaTime * velocity;

	// Apply collision response
	if (position.y - 1.0f < 0 && velocity.y < 0) // I'm lazy :(
	{
		velocity.y *= -1.0f;
	}

	// Apply gravity
	glm::vec3 acceleration(0, -9.81f, 0);

	// Update velocity
	velocity += deltaTime * acceleration;
}
