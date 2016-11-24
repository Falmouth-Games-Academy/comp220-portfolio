#pragma once

#include <vector>
#include "noise.h"
#include "Mesh.h"

class Terrain
{
public:
	Terrain(int iterations, Mesh &passedMesh);

	~Terrain();

	void generateTerrain(int maxX, int maxY, float noiseAmplification, float heightAmplification);

	double getLayeredNoise(double nx, double ny);

private:
	std::vector<noise::Perlin> noiseArray;

	Mesh *mesh;
};

