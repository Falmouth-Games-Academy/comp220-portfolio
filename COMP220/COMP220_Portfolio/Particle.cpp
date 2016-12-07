#include "stdafx.h"
#include "Particle.h"


Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, Plane* levelFloor)
	: position(initialPosition), velocity(initialVelocity), floor(levelFloor)
{
}


Particle::~Particle()
{
}

void Particle::update(float deltaTime)
{
	// Apply collision response
	if (position.y - size < floor->getY() && velocity.y < 0)
	{
		velocity.y *= -0.4f;
	}

	position += deltaTime * velocity;
	// Apply gravity
	glm::vec3 acceleration(0, -4.0f, 0);

	// Update velocity
	velocity += deltaTime * acceleration;

}
