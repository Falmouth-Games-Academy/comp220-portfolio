#include "stdafx.h"
#include "Renderer.h" // Vertex (might be moved laer on)

#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Mesh {
public:
	Mesh() : vertices(nullptr), numVertices(0), indices(nullptr), numIndices(0) {}

	Mesh(const char* filename) {
		Create(filename);
	}

	~Mesh() {
		delete vertices;
		delete indices;
	}

public:
	// Loads the mesh from a file
	bool Create(const char* filename);

private:
	// Array of the vertices in this mesh
	Vertex* vertices;
	unsigned int numVertices;

	// Array of the vertex indices in this mesh
	unsigned int* indices;
	unsigned int numIndices;
};

bool Mesh::Create(const char* filename) {
	// Load the scene from the file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene) {
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	// Copy the vertices and indices from the scene
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh *currentMesh = scene->mMeshes[i];

		// Load vertices
		for (int v = 0; v < currentMesh->mNumVertices; v++) {
			aiVector3D currentModelVertex = currentMesh->mVertices[v];
			aiColor4D currentModelColour = aiColor4D(1.0, 1.0, 1.0, 1.0);
			aiVector3D currentTextureCoordinates = aiVector3D(0.0f, 0.0f, 0.0f);

			if (currentMesh->HasVertexColors(0)) {
				currentModelColour = currentMesh->mColors[0][v];
			}
			
			if (currentMesh->HasTextureCoords(0)) {
				currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
			}

			Vertex currentVertex = {
				currentModelVertex.x, currentModelVertex.y, currentModelVertex.z,
				currentModelColour.r, currentModelColour.g, currentModelColour.b,
				currentTextureCoordinates.x, currentTextureCoordinates.y
			};

			vertices.push_back(currentVertex);
		}

		// Load faces/vertex indices
		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];

			indices.push_back(currentModelFace.mIndices[0]);
			indices.push_back(currentModelFace.mIndices[1]);
			indices.push_back(currentModelFace.mIndices[2]);
		}
	}

	// Copy the results to our array
	this->vertices = new Vertex[vertices.size()];
	this->indices = new unsigned int[indices.size()];
	this->numVertices = vertices.size();
	this->numIndices = indices.size();

	memcpy(this->vertices, vertices.data(), vertices.size() * sizeof(this->vertices[0]));
	memcpy(this->indices, indices.data(), indices.size() * sizeof(this->indices[0]));

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);*/

	return true;
}
