#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::makeGrid()
{
	// Temp y value before perlin noise
	float y = 0;
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
void Terrain::generateTerrain(Mesh& groundTexture, Mesh& snowTexture)
{
	// Generate perlin noise from seed
	PerlinNoise perlinNoise;
	int seed = SDL_GetTicks();
	perlinNoise.GenerateNoise(seed);

	std::string face = "test";

	//Makes the grid of voxels
	makeGrid();

	for (float x = 0; x < terrainWidth; x++)
	{
		for (float z = 0; z < terrainDepth; z++)
		{
			// Calculate terrain height based of position
			float terrainHeight = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), 0);
			terrainHeight = (char)((terrainHeight - noiseMin) * (255 / (noiseMax - noiseMin)));
			glm::vec3 voxelPos(x + voxelSize, terrainHeight + voxelSize, z + voxelSize);
			
			
			// Calculate neighbouring voxels and remove unnessary triangles
			for each (auto voxel in getNeighbourVoxels(voxelPos, Voxels))
			{
				if (voxel.isVoxel)
				{
					Voxels[x][z].placeVoxel(groundTexture, snowTexture, voxelPos);
				}
			}

			
			


		}

	}
}

std::vector<Voxel> Terrain::getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels)
{
	// Vector to return
	std::vector<Voxel> result;
	float xPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().x;
	float zPos = Voxels[voxelPosition.x][voxelPosition.z].getvoxelPosition().z;

	Voxel northVoxel(xPos, voxelPosition.y, zPos + voxelSize);
	Voxel eastVoxel(voxelPosition.x + voxelSize, voxelPosition.y, voxelPosition.z);
	Voxel southVoxel(voxelPosition.x, voxelPosition.y, voxelPosition.z - voxelSize);
	Voxel westVoxel(voxelPosition.x - voxelSize, voxelPosition.y, voxelPosition.z);
	Voxel topVoxel(voxelPosition.x, voxelPosition.y + voxelSize, voxelPosition.z);
	Voxel bottomVoxel(voxelPosition.x, voxelPosition.y - voxelSize, voxelPosition.z);

	result.push_back(northVoxel);
	result.push_back(eastVoxel);
	result.push_back(southVoxel);
	result.push_back(westVoxel);
	result.push_back(topVoxel);
	result.push_back(bottomVoxel);
	return result;
}

void Terrain::generateChunk(Mesh& grassMesh, Mesh& mountainMesh)
{
	//Create instance of class
	PerlinNoise perlinNoise;

	// Geneate perlin noise based of Ken Perlins Permutation Vector
	//perlinNoise.GeneratePerlinNoise();


	// Used for third dimension of perlin noise
	int seed = SDL_GetTicks();

	// Generate perlin noise based off a seed
	perlinNoise.GenerateNoise(seed);

	// The grounds colour Variable
	glm::vec3 colour = glm::vec3(0, 0, 0);

	for (int x = 0; x < terrainWidth; x++)
	{
		for (int z = 0; z < terrainDepth; z++)
		{
			double perlinResult = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), y);
			double lastPerlinResult = perlinResult;

			//Normalize values
			perlinResult = (char)((perlinResult - noiseMin) * (255 / (noiseMax - noiseMin)));;

			// Cube Colour
			if (perlinResult > 0)
				colour = glm::vec3(perlinResult / 100, perlinResult / 100, perlinResult / 100);
			else
				colour = glm::vec3(0.01, 0.02, 0.01);
			
			// Reduce the incline of the slope if it is below 0
			if (perlinResult <= snowPeakHeight)
			{
				// TODO: Remove the unnessary squares to form a mesh
				glm::vec3 a(x - SquareSize, perlinResult * GoundAmplification + SquareSize, z + SquareSize);
				glm::vec3 b(x + SquareSize, perlinResult * GoundAmplification + SquareSize, z + SquareSize);
				glm::vec3 c(x + SquareSize, perlinResult * GoundAmplification + SquareSize, z - SquareSize);
				glm::vec3 d(x - SquareSize, perlinResult * GoundAmplification + SquareSize, z - SquareSize);
				glm::vec3 e(x - SquareSize, perlinResult * GoundAmplification - SquareSize, z + SquareSize);
				glm::vec3 f(x - SquareSize, perlinResult * GoundAmplification - SquareSize, z - SquareSize);
				glm::vec3 g(x + SquareSize, perlinResult * GoundAmplification - SquareSize, z - SquareSize);
				glm::vec3 h(x + SquareSize, perlinResult * GoundAmplification - SquareSize, z + SquareSize);
				
				grassMesh.addCube(a, b, c, d, e, f, g, h, colour);
			}

			// If the perlin noise is above the snow peak, render a differnet cube texture
			else if (perlinResult >= snowPeakHeight)
			{
				glm::vec3 a(x - SquareSize, perlinResult + SquareSize, z + SquareSize);
				glm::vec3 b(x + SquareSize, perlinResult + SquareSize, z + SquareSize);
				glm::vec3 c(x + SquareSize, perlinResult + SquareSize, z - SquareSize);
				glm::vec3 d(x - SquareSize, perlinResult + SquareSize, z - SquareSize);
				glm::vec3 e(x - SquareSize, perlinResult - SquareSize, z + SquareSize);
				glm::vec3 f(x - SquareSize, perlinResult - SquareSize, z - SquareSize);
				glm::vec3 g(x + SquareSize, perlinResult - SquareSize, z - SquareSize);
				glm::vec3 h(x + SquareSize, perlinResult - SquareSize, z + SquareSize);

				mountainMesh.addCube(a, b, c, d, e, f, g, h, colour);
			}
		}
	}
}