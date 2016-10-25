#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
//acts as a struct but will be used in the future
class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
	}

	inline glm::vec3* GetPos() { return &pos; }		//gets private vec3 pos
	inline glm::vec2* GetTexCoord() { return &texCoord; }//gets private vec2 Coord

protected:
private:
	glm::vec3 pos;
	glm::vec2 texCoord; //the posistion on the texture we will be mapping to the vertex
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
		TEXCOORD_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount; //how much of vertexarry object we want to draw




};

#endif MESH_H