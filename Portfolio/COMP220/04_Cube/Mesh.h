#pragma once
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Triangle made from vertex class
	void addTriangleVertex(Vertex v1, Vertex v2, Vertex v3);

	// Old triangle function
	void addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec3& colour,
		const glm::vec2& t1, const glm::vec2& t2, const glm::vec2& t3);
	void addSquare(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d,
		const glm::vec3& colour,
		float u1, float u2, float v1, float v2);

	void Mesh::addCube(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& e, const glm::vec3& f, const glm::vec3& g, const glm::vec3& h, const glm::vec3& colour);

	void addCircle(const glm::vec3& centre, float radius, int numPoints,
		const glm::vec3& colour);

	void addCylinder(const glm::vec3& centre, float radius, int numPoints, float height,
		const glm::vec3& colour);

	void createBuffers();
	void draw();

private:
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	void addVertex(const Vertex& vertex);
};
