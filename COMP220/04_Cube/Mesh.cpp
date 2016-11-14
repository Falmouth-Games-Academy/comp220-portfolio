#include "stdafx.h"
#include "Mesh.h"
#include "noise.h"
#include <time.h>
#include <cmath>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	if (m_positionBuffer != 0)
		glDeleteBuffers(1, &m_positionBuffer);

	if (m_colourBuffer != 0)
		glDeleteBuffers(1, &m_colourBuffer);

	if (m_uvBuffer != 0)
		glDeleteBuffers(1, &m_uvBuffer);
}

void Mesh::addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
	const glm::vec3& colour,
	const glm::vec2& t1, const glm::vec2& t2, const glm::vec2& t3)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add triangles after createBuffers() has been called");
	}

	// Add the vertex positions
	m_vertexPositions.push_back(p1);
	m_vertexPositions.push_back(p2);
	m_vertexPositions.push_back(p3);
	
	// Make all three vertices the same colour
	for (int i = 0; i < 3; i++)
		m_vertexColours.push_back(colour);

	// Add the texture coordinates
	m_vertexUVs.push_back(t1);
	m_vertexUVs.push_back(t2);
	m_vertexUVs.push_back(t3);
}

void Mesh::addSquare(const glm::vec3& a, const glm::vec3& b,
	const glm::vec3& c, const glm::vec3& d, const glm::vec3& colour,
	float u1, float u2, float v1, float v2)
{
	glm::vec2 ta(u1, v1);
	glm::vec2 tb(u1, v2);
	glm::vec2 tc(u2, v2);
	glm::vec2 td(u2, v1);

	addTriangle(a, b, d, colour, ta, tb, td);
	addTriangle(d, b, c, colour, td, tb, tc);
}

void Mesh::addCircle(const glm::vec3& centre, float radius, int numPoints,
	const glm::vec3& colour)
{
	float angleStep = glm::radians(360.0f) / numPoints;

	glm::vec3 lastEdgePoint = centre + radius * glm::vec3(1, 0, 0);

	for (int i = 1; i <= numPoints; i++)
	{
		float angle = angleStep * i;
		glm::vec3 nextEdgePoint = centre + radius * glm::vec3(cos(angle), 0, sin(angle));
		addTriangle(centre, nextEdgePoint, lastEdgePoint, colour, glm::vec2(), glm::vec2(), glm::vec2());
		lastEdgePoint = nextEdgePoint;
	}
}



void Mesh::generateTerrain(int maxX, int maxY, float noiseAmplification, float heightAmplification)
{
	uint32_t seed = time(0);
	const noise::Perlin perlin(seed);
	double previousNx = 0, previousNy = 0;
	for (double x = 1; x < maxX; x++)
	{
		double nx = x / maxX * noiseAmplification;
		for (double y = 1; y < maxY; y++)
		{
			double ny = y / maxY * noiseAmplification;

			glm::vec3 p1(x - 1, getLayeredNoise(previousNx, previousNy, perlin, 1) * heightAmplification, y - 1);
			glm::vec3 p2(x, getLayeredNoise(nx, previousNy, perlin, 1) * heightAmplification, y - 1);
			glm::vec3 p3(x, getLayeredNoise(nx, ny, perlin, 1) * heightAmplification, y);
			glm::vec3 p4(x - 1, getLayeredNoise(previousNx, ny, perlin, 1) * heightAmplification, y);

			glm::vec3 col(getLayeredNoise(nx, ny, perlin, 4) + 0.5, 1 - (getLayeredNoise(nx, ny, perlin, 4) + 0.5), (getLayeredNoise(nx, ny, perlin, 4) + 0.5) * ((nx + ny) / 2));

			addSquare(p4, p3, p2, p1, col, -1, +1, -1, +1);

			previousNy = ny;

		}

		previousNx = nx;
	}
}

// Layers higher frequencys of noise over original generation for more varience
double Mesh::getLayeredNoise(double nx, double ny, noise::Perlin perlin, int numberOfIterations)
{
	double layerOpacity = 1;
	double exponenial = 1;
	double noise = 0;

	for (int i = 0; i < numberOfIterations; i++)
	{
		layerOpacity /= 2;
		noise += layerOpacity * perlin.noise(exponenial * nx, exponenial * ny);
		exponenial *= 2;
	}
	return noise;
}

void Mesh::createBuffers()
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("createBuffers() has already been called");
	}

	// Create and fill the position buffer
	glGenBuffers(1, &m_positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexPositions.size() * sizeof(glm::vec3), m_vertexPositions.data(), GL_STATIC_DRAW);

	// Create and fill the colour buffer
	glGenBuffers(1, &m_colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexColours.size() * sizeof(glm::vec3), m_vertexColours.data(), GL_STATIC_DRAW);

	// Create and fill the texture coordinate buffer
	glGenBuffers(1, &m_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexUVs.size() * sizeof(glm::vec2), m_vertexUVs.data(), GL_STATIC_DRAW);
}

void Mesh::draw()
{
	if (m_positionBuffer == 0)
	{
		throw std::exception("createBuffers() must be called before draw()");
	}

	// Bind the position buffer to vertex attribute 0
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the colour buffer to vertex attribute 1
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Bind the texture coordinate buffer to vertex attribute 2
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glDrawArrays(GL_TRIANGLES, 0, m_vertexPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


