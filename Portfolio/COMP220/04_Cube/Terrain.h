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

	//! Places a cube
	void placeCube(Mesh& grassMesh, Mesh& mountainMesh, glm::vec3& voxelPosition, std::vector<std::vector<Voxel>>& Voxels);

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
	const float voxelSize = voxel.getVoxelSize();

	//! Getters
	const float getTerrainWidth() { return terrainWidth; }
	const float getTerrainDepth() { return terrainDepth; }
	const float getGroundAmplification() { return GoundAmplification; }

private:
	const float terrainWidth = 300;
	const float terrainDepth = 300;
	const float noiseMax = 3;
	const float noiseMin = 0;
	const float SquareSize = 1.5f;

	//! Amplification(the lower the number the higher the amplification)
	const float noiseAmplification = 180.0;

	//! Changes at what point the peaks change to snow
	const float snowPeakHeight = 10.0f;

	// !Changes how varied the ground is
	const float GoundAmplification = 1.0f;

};

