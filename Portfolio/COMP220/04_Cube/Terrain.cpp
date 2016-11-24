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
	for (int x = 0; x < terrainWidth; x++)
	{
		std::vector<Voxel> column;
		Voxels.push_back(column);

		for (int z = 0; z < terrainDepth; z++)
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

	for (float x = 0; x < terrainWidth; x++)
	{
		for (float z = 0; z < terrainDepth; z++)
		{
			// Calculate terrain height based of position
			float terrainHeight = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), 0);
			terrainHeight = (char)((terrainHeight - noiseMin) * (255 / (noiseMax - noiseMin)));
			glm::vec3 voxelPos(x + voxelSize + voxelSize, terrainHeight + voxelSize + voxelSize, z + voxelSize + voxelSize);


			/* TODO: Calculate neighbouring voxels and remove unnessary triangles
			for each (auto voxel in getNeighbourVoxels(voxelPos, Voxels))
			{
				// Code to only place the right cube faces goes here
			}
			*/

			// Place the voxel
			Voxels[x][z].placeVoxel(groundTexture, snowTexture, voxelPos);
			
		}
	}
}


//TODO: generate chunks when the player moves in that direction
void Terrain::generateChunk(Mesh& grassMesh, Mesh& mountainMesh)
{
	
}

// Gets neighbouring voxels around a voxel
std::vector<Voxel> Terrain::getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels)
{
	// Vector to return
	std::vector<Voxel> result;
	float xPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().x;
	float zPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().z;

	Voxel northVoxel(xPos, voxelPosition.y, zPos + voxelSize + voxelSize);
	Voxel eastVoxel(xPos + voxelSize + voxelSize, voxelPosition.y, zPos);
	Voxel southVoxel(xPos, voxelPosition.y, zPos - voxelSize);
	Voxel westVoxel(xPos - voxelSize + voxelSize, voxelPosition.y, zPos);
	Voxel topVoxel(xPos, voxelPosition.y + voxelSize + voxelSize, zPos);
	Voxel bottomVoxel(xPos, voxelPosition.y - voxelSize + voxelSize, zPos);

	result.push_back(northVoxel);
	result.push_back(eastVoxel);
	result.push_back(southVoxel);
	result.push_back(westVoxel);
	result.push_back(topVoxel);
	result.push_back(bottomVoxel);
	return result;
}