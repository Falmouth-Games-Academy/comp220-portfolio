#pragma once
#include"Mesh.h"
#include"Texture.h"
#include"ErrorMessage.h"
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool loadOBJ(const char * path, glm::vec3 modelColour);

	//std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temporaryVertices, temporaryNormals;
	std::vector< glm::vec2 > temporaryUvs;
	
	//! Loads the textures to be applied to the meshes
	void loadTextures(const std::string& fileName);
	//! Checks that enough textures have been loaded 
	void checkTextures();

	glm::vec3 colour;

	ErrorMessage errorSystem;

	int currentMesh = 0;
	std::vector<Mesh> modelMeshes;
	std::vector<Texture> modelTextures;

	void addtoVector();

	unsigned int vertexIndex = 0, uvIndex = 0, normalIndex = 0;
};

