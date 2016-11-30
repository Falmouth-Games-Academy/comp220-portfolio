#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
	//TODO: Delete terrain chunks after a set distance
}

void Terrain::makeGrid()
{
	// Temp y value before perlin noise
	float y = 0;

	// Populate the grid with Voxels
	for (float x = 0; x < terrainWidth + 2000; x++)
	{
		std::vector<Voxel> column;
		Voxels.push_back(column);

		for (float z = 0; z < terrainDepth + 2000; z++)
		{
			Voxel voxel(x, y, z);
			Voxels[x].push_back(voxel);
			
		}
	}
}

float Terrain::getHeight(float x, float z)
{
	float height = Voxels[x][z].getvoxelPosition().y;
	return height;
}
void Terrain::generateTerrain(Mesh& groundTexture, Mesh& snowTexture)
{
	// Generate perlin noise from seed
	PerlinNoise perlinNoise;
	int seed = SDL_GetTicks();
	perlinNoise.GenerateNoise(seed);

	// Makes the grid of voxels
	makeGrid();
	glm::vec3 lastVoxelPos(0, 0, 0);
	for (float x = 0; x < terrainWidth; x++)
	{
		for (float z = 0; z < terrainDepth; z++)
		{
			// Calculate terrain height based of position
			float terrainHeight = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), 0);
			terrainHeight = (char)((terrainHeight - noiseMin) * (255 / (noiseMax - noiseMin)));
			glm::vec3 voxelPos(x + voxelSize + voxelSize, terrainHeight + voxelSize + voxelSize, z + voxelSize + voxelSize);


			/* TODO: Calculate neighbouring voxels and remove unnessary triangles
			*/
			
			
			//placeCube(groundTexture, snowTexture, voxelPos, Voxels, lastVoxelPos);
			// Place the voxel
			Voxels[x][z].placeVoxel(groundTexture, snowTexture, voxelPos);
			lastVoxelPos.x = voxelPos.x;
		}
	}
}


//TODO: generate chunks when the player moves in that direction
void Terrain::generateChunk(Mesh& grassMesh, Mesh& mountainMesh)
{
	
}

void Terrain::placeCube(Mesh& grassMesh, Mesh& mountainMesh, glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels, glm::vec3& lastVoxelPosition)
{
	glm::vec3 colour(0.25, 0.25, 0.25);
	std::vector<glm::vec3> faces;
	if (voxelPosition.x == lastVoxelPosition.x)
	{
		glm::vec3 null(0, 0, 0);
		for (int i = 0; i < 6; i++)
			faces.push_back(null);

		grassMesh.addCubeFromFace(faces, colour);
	}
	else
	{
		if (voxelPosition.y < 0)
		{
			
			glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
			
			faces.push_back(a);
			faces.push_back(b);
			faces.push_back(c);
			faces.push_back(d);
			faces.push_back(e);
			faces.push_back(f);
			faces.push_back(g);
			faces.push_back(h);


			grassMesh.addCubeFromFace(faces, colour);
		}
		// Else render snow texture cube
		else
		{
			glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
			glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
			glm::vec3 colour(voxelPosition.y / 50, voxelPosition.y / 50, voxelPosition.y / 50);

			faces.push_back(a);
			faces.push_back(b);
			faces.push_back(c);
			faces.push_back(d);
			faces.push_back(e);
			faces.push_back(f);
			faces.push_back(g);
			faces.push_back(h);

			grassMesh.addCubeFromFace(faces, colour);
		}
	}
	/*
	for each (auto voxel in getNeighbourVoxels(voxelPosition, Voxels))
	{

	}
	*/
}

// Gets neighbouring voxels around a voxel
std::vector<Voxel> Terrain::getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels)
{
	// Vector to return
	std::vector<Voxel> result;
	float xPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().x;
	float zPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().z;

	Voxel northVoxel(xPos, voxelPosition.y, zPos + voxelSize);
	Voxel eastVoxel(xPos + voxelSize, voxelPosition.y, zPos);
	Voxel southVoxel(xPos, voxelPosition.y, zPos - voxelSize);
	Voxel westVoxel(xPos - voxelSize, voxelPosition.y, zPos);
	Voxel topVoxel(xPos, voxelPosition.y + voxelSize, zPos);
	Voxel bottomVoxel(xPos, voxelPosition.y - voxelSize, zPos);

	result.push_back(northVoxel);
	result.push_back(eastVoxel);
	result.push_back(southVoxel);
	result.push_back(westVoxel);
	result.push_back(topVoxel);
	result.push_back(bottomVoxel);
	return result;
}