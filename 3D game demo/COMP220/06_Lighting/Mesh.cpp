#include "stdafx.h"
#include "mesh.h"

mesh::mesh()
{
}

mesh::~mesh()
{
	if (m_posBuffer != 0)
		glDeleteBuffers(1, &m_posBuffer);

	if (m_colourBuffer != 0)
		glDeleteBuffers(1, &m_colourBuffer);

	if (m_uvBuffer != 0)
		glDeleteBuffers(1, &m_uvBuffer);

	if (m_normalBuffer != 0)
		glDeleteBuffers(1, &m_normalBuffer);
}

void mesh::triangle(Vertex v1, Vertex v2, Vertex v3)
{
	glm::vec3 normal = glm::cross(v2.m_position - v1.m_position, v3.m_position - v1.m_position);
	normal = glm::normalize(normal);

	v1.m_normal = normal;
	v2.m_normal = normal;
	v3.m_normal = normal;

	vertex(v1);
	vertex(v2);
	vertex(v3);
}

void mesh::nonWall()
{
	int nonWall = 0;
}

void mesh::wall(const glm::vec3& a, const glm::vec3& b,
	const glm::vec3& c, const glm::vec3& d, const glm::vec3& colour,
	float u1, float u2, float v1, float v2)
{
	glm::vec2 ta(u1, v1);
	glm::vec2 tb(u1, v2);
	glm::vec2 tc(u2, v2);
	glm::vec2 td(u2, v1);

	triangle(a, b, d);
	triangle(d, b, c);
}

void mesh::vertex(const Vertex& vertex)
{
	if (m_posBuffer != 0)
	{
		throw std::exception("Cannot add vertices after createBuffers() has been called");
	}

	m_vertexPositions.push_back(vertex.m_position);
	m_vertexColours.push_back(vertex.m_colour);
	m_vertexUVs.push_back(vertex.m_textureCoord);
	m_vertexNormals.push_back(vertex.m_normal);
}


void mesh::createBuffers()
{
	if (m_posBuffer != 0)
	{
		throw std::exception("createBuffers() has already been called");
	}

	// Create and fill the position buffer
	glGenBuffers(1, &m_posBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexPositions.size() * sizeof(glm::vec3), m_vertexPositions.data(), GL_STATIC_DRAW);

	// Create and fill the colour buffer
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexColours.size() * sizeof(glm::vec3), m_vertexColours.data(), GL_STATIC_DRAW);

	// Create and fill the texture coordinate buffer
	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexUVs.size() * sizeof(glm::vec2), m_vertexUVs.data(), GL_STATIC_DRAW);

	// Create and fill the normal buffer
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * sizeof(glm::vec3), m_vertexNormals.data(), GL_STATIC_DRAW);
}

void mesh::draw()
{
	if (m_posBuffer == 0)
	{
		throw std::exception("createBuffers() must be called before draw()");
	}

	// Bind the position buffer to vertex attribute 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_posBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the colour buffer to vertex attribute 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the texture coordinate buffer to vertex attribute 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the normal buffer to vertex attribute 3
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}
