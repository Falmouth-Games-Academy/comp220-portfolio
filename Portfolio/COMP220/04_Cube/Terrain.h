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
	
	//! Generates a chunk (in-progress)
	void generateChunk(Mesh& grassMesh, Mesh& mountainMesh);

	//! Generates the terrain
	void generateTerrain(Mesh& mesh, Mesh& snowTexture);

	//! Creates a grid of voxels
	void makeGrid();

	//! Checks what the neighbouring cells are
	std::vector<Voxel> getNeighbourVoxels(glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels);

	//! Returns the height of the voxel based on x and z values
	float getHeight(float x, float z);

	//! Vector of vectors of voxels
	std::vector<std::vector<Voxel>> Voxels;

	//! Get voxel size
	Voxel voxel;
	float voxelSize = voxel.getVoxelSize();

	//! Getters
	int getTerrainWidth() { return terrainWidth; }
	int getTerrainDepth() { return terrainDepth; }
	int getGroundAmplification() { return GoundAmplification; }

private:
	int terrainWidth = 500;
	int terrainDepth = 500;
	int noiseMax = 3;
	int noiseMin = 0;
	float SquareSize = 1.5f;

	//! Amplification(the lower the number the higher the amplification)
	float noiseAmplification = 180.0;

	//! Changes at what point the peaks change to snow
	float snowPeakHeight = 10.0f;

	// !Changes how varied the ground is
	float GoundAmplification = 1.0f;

};

