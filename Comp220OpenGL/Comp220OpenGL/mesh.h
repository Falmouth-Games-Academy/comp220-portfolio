#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "objLoader.h"

//acts as a struct but will be used in the future
class Vertex
{ 
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal = glm::vec3(0,0,0))
	{
		this->pos = pos; // pointer is used to retrieve the object's pos
		this->texCoord = texCoord;// pointer is used to retrieve the object's texCoord
		this->normal = normal;// pointer is used to retrieve the object's normal
	}

	inline glm::vec3* GetPos() { return &pos; }		//gets private vec3 pos
	inline glm::vec2* GetTexCoord() { return &texCoord; }//gets private vec2 Coord
	inline glm::vec3* GetNormal() { return &normal; }   //gets the normal

protected:
private:
	glm::vec3 pos;
	glm::vec2 texCoord; //the posistion on the texture we will be mapping to the vertex
	glm::vec3 normal;	//the up direction on a surface
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);//specify the draw order
	Mesh(const std::string& fileName);

	void Draw();

	virtual ~Mesh();

protected:
private:
	Mesh(const Mesh& other);
	void operator=(const Mesh& other);

	void InitMesh(const IndexedModel& model);

	enum
	{
		//storing normals
		POSISTION_VB,
		TEXCOORD_VB,
		NORMAL_VB,	
		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount; //how much of vertexarry object we want to draw




};

#endif MESH_H