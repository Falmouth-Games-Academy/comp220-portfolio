#include "stdafx.h"
#include "Vertex.h"


Vertex::Vertex(const glm::vec3& position, const glm::vec3& colour,
	const glm::vec2& textureCoord, const glm::vec3& normal)
{
	vertexPosition = position;
	vertexColour = colour;
	vertexTextureCoord = textureCoord;
	vertexNormal = normal;
}