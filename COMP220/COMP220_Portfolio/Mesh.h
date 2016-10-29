#pragma once

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addTriangle(Vertex v1, Vertex v2, Vertex v3);

	void addSphere(float radius, int quality, const glm::vec3& colour);
	
	bool Mesh::loadOBJ(const char * path);

	void createBuffers();
	void draw();

private:

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temporaryVertices, temporaryNormals;
	std::vector< glm::vec2 > temporaryUvs;

	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	void addVertex(const Vertex& vertex);

	unsigned int vertexIndex = 0, uvIndex = 0, normalIndex = 0;

	Vertex createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour);
};