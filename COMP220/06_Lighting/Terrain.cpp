#include "stdafx.h"
#include "Terrain.h"
#include "time.h"

Terrain::Terrain(int iterations, Mesh &passedMesh)
{
	for (int i = 0; i < iterations; i++)
	{
		noiseArray.push_back(noise::Perlin(time(0) + iterations * i));
	}

	mesh = &passedMesh;
}

void Terrain::generateTerrain(int maxX, int maxY, float noiseAmplification, float heightAmplification)
{
	double previousNx = 0, previousNy = 0;
	for (double x = 1; x < maxX; x++)
	{
		double nx = x / maxX * noiseAmplification;
		for (double y = 1; y < maxY; y++)
		{
			double ny = y / maxY * noiseAmplification;

			//vertex positions
			glm::vec3 p1(x - 1, getLayeredNoise(previousNx, previousNy) * heightAmplification, y - 1);
			glm::vec3 p2(x, getLayeredNoise(nx, previousNy) * heightAmplification, y - 1);
			glm::vec3 p3(x, getLayeredNoise(nx, ny) * heightAmplification, y);
			glm::vec3 p4(x - 1, getLayeredNoise(previousNx, ny) * heightAmplification, y);

			Vertex v1(p1, glm::vec3(0.5, 1, 1));
			Vertex v2(p2, glm::vec3(1, 0.5, 1));
			Vertex v3(p3, glm::vec3(1, 1, 0.5));
			Vertex v4(p4, glm::vec3(1, 1, 1));

			glm::vec3 col(getLayeredNoise(nx, ny) + 0.5, 1 - (getLayeredNoise(nx, ny) + 0.5), (getLayeredNoise(nx, ny) + 0.5) * ((nx + ny) / 2));

			mesh->addSquare(v4, v3, v2, v1);

			previousNy = ny;

		}

		previousNx = nx;
	}
	xMax = maxX;
	yMax = maxY;
	noiseAmp = noiseAmplification;
	heightAmp = heightAmplification;
}

// Layers higher frequencys of noise over original generation for more varience
double Terrain::getLayeredNoise(double nx, double ny)
{
	int numberOfIterations = noiseArray.size();
	double layerOpacity = 1;
	double exponenial = 1;
	double noise = 0;

	for (int i = 0; i < numberOfIterations; i++)
	{
		layerOpacity /= 2;
		noise += layerOpacity * noiseArray[i].noise(exponenial * nx, exponenial * ny);
		exponenial *= 2;
	}
	return noise;
}


Terrain::~Terrain()
{
}
