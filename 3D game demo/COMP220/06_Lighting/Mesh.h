#pragma once
#include "stdafx.h"
#include "Vertex.h"

class mesh
{
public:
	mesh();
	~mesh();

	void triangle(Vertex v1, Vertex v2, Vertex v3);

	void wall(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d,
		const glm::vec3& colour,
		float u1, float u2, float v1, float v2);

	void nonWall();
	void createBuffers();
	void draw();

private:
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_posBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	void vertex(const Vertex& vertex);
};
