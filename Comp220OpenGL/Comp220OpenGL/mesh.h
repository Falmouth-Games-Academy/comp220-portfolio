#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
//acts as a struct but will be used in the future
class Vertex
{
public:
	Vertex(const glm::vec3& pos)
	{
		this->pos = pos;
	}
protected:
private:
	glm::vec3 pos;
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices);


	void Draw();

	virtual ~Mesh();

protected:
private:
	Mesh(const Mesh& other);
	void operator=(const Mesh& other);

	enum
	{
		POSISTION_VB,

		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount; //how much of vertexarry object we want to draw




};

#endif MESH_H