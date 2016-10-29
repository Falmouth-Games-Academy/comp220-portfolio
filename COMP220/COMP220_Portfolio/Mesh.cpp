#include "stdafx.h"
#include "Mesh.h"

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

	if (m_normalBuffer != 0)
		glDeleteBuffers(1, &m_normalBuffer);
}

void Mesh::addTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	glm::vec3 normal = glm::cross(v2.vertexPosition - v1.vertexPosition, v3.vertexPosition - v1.vertexPosition);
	normal = glm::normalize(normal);

	v1.vertexNormal = normal;
	v2.vertexNormal = normal;
	v3.vertexNormal = normal;

	addVertex(v1);
	addVertex(v2);
	addVertex(v3);
}

void Mesh::addVertex(const Vertex& vertex)
{
	if (m_positionBuffer != 0)
	{
		throw std::exception("Cannot add vertices after createBuffers() has been called");
	}

	m_vertexPositions.push_back(vertex.vertexPosition);
	m_vertexColours.push_back(vertex.vertexColour);
	m_vertexUVs.push_back(vertex.vertexTextureCoord);
	m_vertexNormals.push_back(vertex.vertexNormal);
}

Vertex Mesh::createSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour)
{
	glm::vec3 unitPos(
		cos(latitude) * cos(longitude),
		sin(latitude),
		cos(latitude) * sin(longitude));

	glm::vec2 textureCoords(
		-longitude / glm::radians(360.0f),
		latitude / glm::radians(180.0f) + 0.5f);

	return Vertex(radius * unitPos,
		colour,
		textureCoords);
}

void Mesh::addSphere(float radius, int quality, const glm::vec3& colour)
{
	float angleStep = glm::radians(90.0f) / quality;

	std::vector<Vertex> lastRingPoints, ringPoints;

	// Top cap
	float latitude = angleStep * (quality - 1);
	for (int i = 0; i <= quality * 4; i++)
	{
		float longitude = i * angleStep;
		ringPoints.push_back(createSphereVertex(radius, longitude, latitude, colour));
		if (ringPoints.size() > 1)
		{
			Vertex pole = createSphereVertex(radius, longitude - 0.5f*angleStep, glm::radians(90.0f), colour);
			addTriangle(pole, ringPoints[i], ringPoints[i - 1]);
		}
	}

	// Rings
	for (int j = quality - 2; j > -quality; j--)
	{
		lastRingPoints.clear();
		std::swap(lastRingPoints, ringPoints);

		float latitude = angleStep * j;
		for (int i = 0; i <= quality * 4; i++)
		{
			float longitude = i * angleStep;
			ringPoints.push_back(createSphereVertex(radius, longitude, latitude, colour));
			if (ringPoints.size() > 1)
			{
				addTriangle(lastRingPoints[i], ringPoints[i], ringPoints[i - 1]);
				addTriangle(lastRingPoints[i - 1], lastRingPoints[i], ringPoints[i - 1]);
			}
		}
	}

	// Bottom cap
	for (int i = 1; i < ringPoints.size(); i++)
	{
		Vertex pole = createSphereVertex(radius, (i - 0.5f)*angleStep, glm::radians(-90.0f), colour);
		addTriangle(pole, ringPoints[i - 1], ringPoints[i]);
	}
}


bool Mesh::loadOBJ(const char * path, glm::vec3 modelColour)
{

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	// Scan through file
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temporaryVertices.push_back(vertex);
		}


		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temporaryUvs.push_back(uv);
		}

		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temporaryNormals.push_back(normal);
		}

		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by simple parser\n");
				return false;
			}
			

			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}

		}



	} //End while
	vertexIndex = 0;
	uvIndex = 0;
	normalIndex = 0;

	float offset = 3;

	for (int i = 0; i < vertexIndices.size(); i++) {
		vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temporaryVertices[vertexIndex - 1];
		m_vertexPositions.push_back(vertex);
	}
	for (int i = 0; i < vertexIndices.size(); i++) {
		glm::vec3 colour = modelColour;
		m_vertexColours.push_back(colour);
	}
	for (int i = 0; i < uvIndices.size(); i++) {
		uvIndex = uvIndices[i];
		glm::vec2 uv = temporaryUvs[uvIndex - 1];
		m_vertexUVs.push_back(uv);
	}
	for (int i = 0; i < normalIndices.size(); i++) {
		normalIndex = normalIndices[i];
		glm::vec3 normal = temporaryNormals[normalIndex - 1];
		m_vertexNormals.push_back(normal);
	}

}// End loadOBJ

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

	// Create and fill the normal buffer
	glGenBuffers(1, &m_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexNormals.size() * sizeof(glm::vec3), m_vertexNormals.data(), GL_STATIC_DRAW);
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