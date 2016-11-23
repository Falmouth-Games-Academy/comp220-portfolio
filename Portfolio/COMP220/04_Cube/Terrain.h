#pragma once
#include "Perlin_Noise.h"
#include "Mesh.h"
#include "Voxel.h"


class Terrain
{
public:
	Terrain();
	~Terrain();

	void generateChunk(Mesh& grassMesh, Mesh& mountainMesh);
	void generateTerrain(Mesh& mesh);

	std::vector<glm::vec3> VoxelPos;

	// Vector of vectors of voxels
	std::vector<std::vector<Voxel>> Voxels;

	int chunkSize = 1000; // Max 700 squares ~3M
	int terrainWidth = 10;
	int terrainDepth = 15;

	int noiseMax = 3;
	int noiseMin = 0;
	int y = 0;
	float SquareSize = 0.5f;

	// Changes at what point the peaks change to snow
	float snowPeakHeight = 10.0f;

	// Changes how varied the ground is
	float GoundAmplification = 1.0f;

	// Amplification(the lower the number the higher the amplification)
	float noiseAmplification = 150.0;
};

