#pragma once
#include"Mesh.h"
#include"Texture.h"
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool loadOBJ(const char * path, glm::vec3 modelColour);

	//std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temporaryVertices, temporaryNormals;
	std::vector< glm::vec2 > temporaryUvs;
	
	void loadTextures(const std::string& fileName);

	glm::vec3 colour;

	//Mesh mesh;
	int currentMesh = 0;
	std::vector<Mesh> modelMeshes;
	std::vector<Texture> modelTextures;

	void addtoVector();

	unsigned int vertexIndex = 0, uvIndex = 0, normalIndex = 0;
};

