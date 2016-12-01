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
	// Generate perlin noise from seed
	PerlinNoise perlinNoise;
	int seed = SDL_GetTicks();
	perlinNoise.GenerateNoise(seed);

	// Temp y value before perlin noise
	float y = 0;

	// Populate the grid with Voxels
	for (float x = 0; x < terrainWidth; x++)
	{
		std::vector<Voxel> column;
		Voxels.push_back(column);

		for (float z = 0; z < terrainDepth; z++)
		{
			float terrainHeight = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), 0);
			terrainHeight = (char)((terrainHeight - noiseMin) * (255 / (noiseMax - noiseMin)));

			Voxel voxel(x, terrainHeight, z);
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
	

	// Makes the grid of voxels
	makeGrid();


	for (float x = 0; x < terrainWidth; x++)
	{
		for (float z = 0; z < terrainDepth; z++)
		{
			
			glm::vec3 voxelPos(x + voxelSize + voxelSize, getHeight(x,z) + voxelSize + voxelSize, z + voxelSize + voxelSize);


			/* TODO: Calculate neighbouring voxels and remove unnessary triangles
			*/
			
			// Place the voxel
			placeCube(groundTexture, snowTexture, voxelPos, Voxels);
		}
	}
}


//TODO: generate chunks when the player moves in that direction
void Terrain::generateChunk(Mesh& grassMesh, Mesh& mountainMesh)
{
	
}

void Terrain::placeCube(Mesh& grassMesh, Mesh& mountainMesh, glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels)
{
	glm::vec3 colour(0.25, 0.25, 0.25);
	std::vector<glm::vec3> faces;


	// Set voxels below snowPeakHeight to be ground
	if (voxelPosition.y < snowPeakHeight)
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

		// Offset the mountain terrain to be lighter at the top
		glm::vec3 colour(voxelPosition.y / 50, voxelPosition.y / 50, voxelPosition.y / 50);

		faces.push_back(a);
		faces.push_back(b);
		faces.push_back(c);
		faces.push_back(d);
		faces.push_back(e);
		faces.push_back(f);
		faces.push_back(g);
		faces.push_back(h);

		mountainMesh.addCubeFromFace(faces, colour);
	}
}

// Gets neighbouring voxels around a voxel (not used at the moment)
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