#include "stdafx.h"
#include "Plane.h"



Plane::Plane(float floorYPosition, float initialFloorSize, const std::string& floorTexture)
	: y(floorYPosition), floorSize(initialFloorSize)
{
	texture.loadTexture(floorTexture);
	glm::vec3 colour(1, 1, 1);
	Vertex floorVertices[] = {
		Vertex(glm::vec3(-floorSize, y, -floorSize), colour, glm::vec2(0,0)),
		Vertex(glm::vec3(-floorSize, y, +floorSize), colour, glm::vec2(0,1)),
		Vertex(glm::vec3(+floorSize, y, +floorSize), colour, glm::vec2(1,1)),
		Vertex(glm::vec3(+floorSize, y, -floorSize), colour, glm::vec2(1,0)),
	};

	mesh.addSquare(floorVertices[0], floorVertices[1], floorVertices[2], floorVertices[3], colour);
	mesh.createBuffers();
}

Plane::~Plane()
{
}
