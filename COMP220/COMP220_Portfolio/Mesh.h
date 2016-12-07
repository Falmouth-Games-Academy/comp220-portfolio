#pragma once

#include "Vertex.h"
//! Mesh class
/*!
	for creating meshes and rendering them
	
*/
class Mesh
{
public:
	//! Constructor
	Mesh();
	//! Destructor
	~Mesh();

	//! Creates a triangle from 3 vertexes
	void addTriangle(Vertex v1, Vertex v2, Vertex v3);
	//! Creates a triangle from 3 vertexes  adn requires a colour
	void addTriangle(Vertex v1, Vertex v2, Vertex v3, const glm::vec3& colour);
	//! Creates a square from 4 vertexs
	void addSquare(Vertex v1, Vertex v2, Vertex v3, Vertex v4, const glm::vec3& colour);
	//! Creates a sphere
	void addSphere(float radius, int quality, const glm::vec3& colour);
	//! Creates a vertex
	void addVertex(const Vertex& vertex);
	//! Adds vertex properties to buffers
	Vertex createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour);

	//! Creates the buffers for the mesh
	void createBuffers();
	//! Renders the mesh
	void draw();

	//! Vectors contains indicies
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	//! Vectors of vec3s containign positions, colours and normals
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	//! Vectors of vec2s containign UVs
	std::vector<glm::vec2> m_vertexUVs;
	//! GLuints for buffers
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	
};