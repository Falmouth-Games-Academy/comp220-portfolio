#pragma once

class Vertex
{
public:
	Vertex(const glm::vec3& position,
		const glm::vec3& colour = glm::vec3(1, 1, 1),
		const glm::vec2& textureCoord = glm::vec2(0, 0),
		const glm::vec3& normal = glm::vec3(0, 0, 0));

	glm::vec3 vertexPosition;
	glm::vec3 vertexColour;
	glm::vec2 vertexTextureCoord;
	glm::vec3 vertexNormal;
};