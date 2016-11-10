#include "stdafx.h"
#include "OBJLoader.h"


OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

bool OBJLoader::loadOBJ(const char * path, glm::vec3 modelColour)
{

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	colour = modelColour;
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

		else if ((strcmp(lineHeader, "usemtl") == 0) ){
			Mesh addMesh;
			modelMeshes.push_back(addMesh);
			currentMesh = modelMeshes.size() - 1;
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
				modelMeshes[currentMesh].vertexIndices.push_back(vertexIndex[i]);
				modelMeshes[currentMesh].uvIndices.push_back(uvIndex[i]);
				modelMeshes[currentMesh].normalIndices.push_back(normalIndex[i]);
			}
		}



	} //End while
	for (int i = 0; i < modelMeshes.size(); i++)
	{
		currentMesh = i;
		addtoVector();
	}

}// End loadOBJ

void OBJLoader::addtoVector()
{
	vertexIndex = 0;
	uvIndex = 0;
	normalIndex = 0;

	for (int i = 0; i < modelMeshes[currentMesh].vertexIndices.size(); i++) {
		vertexIndex = modelMeshes[currentMesh].vertexIndices[i];
		glm::vec3 vertex = temporaryVertices[vertexIndex - 1];
		modelMeshes[currentMesh].m_vertexPositions.push_back(vertex);
	}
	for (int i = 0; i < modelMeshes[currentMesh].vertexIndices.size(); i++) {
		modelMeshes[currentMesh].m_vertexColours.push_back(colour);
	}
	for (int i = 0; i < modelMeshes[currentMesh].uvIndices.size(); i++) {
		uvIndex = modelMeshes[currentMesh].uvIndices[i];
		glm::vec2 uv = temporaryUvs[uvIndex - 1];
		modelMeshes[currentMesh].m_vertexUVs.push_back(uv);
	}
	for (int i = 0; i < modelMeshes[currentMesh].normalIndices.size(); i++) {
		normalIndex = modelMeshes[currentMesh].normalIndices[i];
		glm::vec3 normal = temporaryNormals[normalIndex - 1];
		modelMeshes[currentMesh].m_vertexNormals.push_back(normal);
	}
}

void OBJLoader::loadTextures(const std::string& fileName)
{
	Texture texture(fileName);
	modelTextures.push_back(texture);
}

void OBJLoader::checkTextures()
{
	if (modelMeshes.size() > modelTextures.size())
		errorSystem.showErrorMessage("Not enough textures loaded.", "Error");
	else if (modelMeshes.size() < modelTextures.size())
		errorSystem.showErrorMessage("Too many textures loaded.", "Error");
}