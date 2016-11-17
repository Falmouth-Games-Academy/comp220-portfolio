#pragma once
#include "Perlin_Noise.h"
#include "Mesh.h"


class Terrain
{
public:
	Terrain();
	~Terrain();

	void generateChunk(Mesh& grassMesh, Mesh& mountainMesh);
	void generateTerrain(Mesh& mesh);


	int chunkSize = 500; // Max 700 squares ~3M
	int noiseMax = 3;
	int noiseMin = 0;
	int y = 0;
	float SquareSize = 0.5f;

	float snowPeakHeight = 3.0f;

	float GoundAmplification = 0.15;

	// Amplification(the lower the number the higher the amplification)
	float noiseAmplification = 150.0;
};

