#pragma once
//! Vertex class
class Vertex
{
public:
	//! Constructor
	Vertex(const glm::vec3& position,
		const glm::vec3& colour = glm::vec3(1, 1, 1),
		const glm::vec2& textureCoord = glm::vec2(0, 0),
		const glm::vec3& normal = glm::vec3(0, 0, 0));

	//! Vertex X,Y,Z coordinates
	glm::vec3 vertexPosition;
	//! Vertex colour
	glm::vec3 vertexColour;
	//! vertex UVs
	glm::vec2 vertexTextureCoord;
	//! Vertex normal 
	glm::vec3 vertexNormal;
};