#pragma once

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	void addSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

	void addSphere(float radius, int quality, const glm::vec3& colour);


	void createBuffers();

	void draw();

	bool LoadObj(
		const char * path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

	void LoadTexture();

private:
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	void addVertex(const Vertex& vertex);

	Vertex createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour);
};
