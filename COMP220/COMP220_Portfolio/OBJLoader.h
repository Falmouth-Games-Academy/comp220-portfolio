#pragma once
#include"Mesh.h"
#include"Texture.h"
#include"ErrorMessage.h"
//! OBJ loading class
/*!
	Loads OBJ models files from a given filepath
*/
class OBJLoader
{
public:
	//! Constructor 
	OBJLoader();
	//! Destructor
	~OBJLoader();

	//! Loads OBJ file
	bool loadOBJ(const char * path, glm::vec3 modelColour);

	//! Temporary vectors of vec3s
	std::vector< glm::vec3 > temporaryVertices, temporaryNormals;
	//! Temporary vector of vec2s
	std::vector< glm::vec2 > temporaryUvs;
	
	//! Loads the textures to be applied to the meshes
	void loadTextures(const std::string& fileName);
	//! Checks that enough textures have been loaded 
	void checkTextures();

	//! Colour to be added to meshes
	glm::vec3 colour;

	//! Error system to create errro windows
	ErrorMessage errorSystem;

	//! Int for determining which mesh is active
	int currentMesh = 0;
	//! Vector of meshes for one model
	std::vector<Mesh> modelMeshes;
	//! Vector of textures for the meshs
	std::vector<Texture> modelTextures;
	//! Vec3 for model position
	glm::vec3 position;
	//! Adds Indices to vector for use in buffers
	void addtoVector();

	//! Unsigned ints for indexes
	unsigned int vertexIndex = 0, uvIndex = 0, normalIndex = 0;
};

