#include "mesh.h"
#include <vector>

//Sends data to the GPU.

Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
	
	m_drawCount = numVertices;
	
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;

	positions.reserve(numVertices);
	texCoords.reserve(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		positions.push_back(*vertices[i].GetPos());
		texCoords.push_back(*vertices[i].GetTexCoord());
	}


	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSISTION_VB]);//will thinhk its an array
	glBufferData(GL_ARRAY_BUFFER,numVertices * sizeof(positions[0]), &positions[0],GL_STATIC_DRAW);//takes some data and puts it into the buffer on gpu
	
	glEnableVertexAttribArray(0);//tells opengl theres going to be an atrtib
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindVertexArray(0);
	//glBindVertexArray(1);
}


Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

	glBindVertexArray(0);
}