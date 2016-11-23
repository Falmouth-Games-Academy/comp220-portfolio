#pragma once

#include "Vertex.h"

class Mesh
{
public:
	//! Constructor
	Mesh();
	//! Destructor
	~Mesh();

	//! Creates a triangle
	void addTriangle(Vertex v1, Vertex v2, Vertex v3);
	void addTriangle(Vertex v1, Vertex v2, Vertex v3, const glm::vec3& colour);
	void addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, const glm::vec3& colour);
	//! Creates a sphere
	void addSphere(float radius, int quality, const glm::vec3& colour);
	void addVertex(const Vertex& vertex);
	Vertex createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour);

	//! Creates all the buffers needs for a mesh
	void createBuffers();
	//! Draws the mesh
	void draw();

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;

	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	
};