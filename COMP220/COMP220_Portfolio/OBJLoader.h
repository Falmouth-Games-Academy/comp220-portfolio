#pragma once
#include"Mesh.h"
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool loadOBJ(const char * path, glm::vec3 modelColour);

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temporaryVertices, temporaryNormals;
	std::vector< glm::vec2 > temporaryUvs;
	Mesh mesh;

	unsigned int vertexIndex = 0, uvIndex = 0, normalIndex = 0;
};

