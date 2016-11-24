#pragma once
#include "Perlin_Noise.h"
#include "Mesh.h"
#include "Voxel.h"
#include "stdafx.h"


class Terrain
{
public:
	Terrain();
	~Terrain();

	void generateChunk(Mesh& grassMesh, Mesh& mountainMesh);
	void generateTerrain(Mesh& mesh, Mesh& snowTexture);
	void makeGrid();
	std::vector<Voxel> getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels);

	// Vector of vectors of voxels
	std::vector<std::vector<Voxel>> Voxels;

	// Get voxel size
	Voxel voxel;
	float voxelSize = voxel.getVoxelSize();
	
	int terrainWidth = 600;
	int terrainDepth = 600;

	int noiseMax = 3;
	int noiseMin = 0;
	int y = 0;
	float SquareSize = 0.5f;

	// Changes at what point the peaks change to snow
	float snowPeakHeight = 10.0f;

	// Changes how varied the ground is
	float GoundAmplification = 1.0f;

	// Amplification(the lower the number the higher the amplification)
	float noiseAmplification = 300.0;
};

