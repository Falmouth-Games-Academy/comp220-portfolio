#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::generateTerrain(Mesh& mesh)
{
	generateChunk(mesh);
}
void Terrain::generateChunk(Mesh& mesh)
{
	//Create instance of class
	PerlinNoise perlinNoise;

	// Geneate perlin noise based of Ken Perlins Permutation Vector
	//perlinNoise.GeneratePerlinNoise();


	// Used for third dimension of perlin noise
	int seed = SDL_GetTicks() / 100;

	// Generate perlin noise based off a seed
	perlinNoise.GenerateNoise(seed);

	// The grounds colour Variable
	glm::vec3 colour = glm::vec3(0, 0, 0);

	for (int x = 0; x < chunkSize; x++)
	{
		for (int z = 0; z < chunkSize; z++)
		{
			double perlinResult = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), y);
			double lastPerlinResult = perlinResult;

			//Normalize values
			perlinResult = (char)((perlinResult - noiseMin) * (255 / (noiseMax - noiseMin)));;

			// Cube Colour
			if (perlinResult > 0)
				colour = glm::vec3(perlinResult / 100, perlinResult / 100, perlinResult / 100);
			else
				colour = glm::vec3(perlinResult / 100, perlinResult / 100, perlinResult / 100);
			
			// Reduce the incline of the slope if it is below 0
			if (perlinResult <= lastPerlinResult )
			{
				glm::vec3 a(x - SquareSize, perlinResult * 0.1 + SquareSize, z + SquareSize);
				glm::vec3 b(x + SquareSize, perlinResult * 0.1 + SquareSize, z + SquareSize);
				glm::vec3 c(x + SquareSize, perlinResult * 0.1 + SquareSize, z - SquareSize);
				glm::vec3 d(x - SquareSize, perlinResult * 0.1 + SquareSize, z - SquareSize);
				glm::vec3 e(x - SquareSize, perlinResult * 0.1 - SquareSize, z + SquareSize);
				glm::vec3 f(x - SquareSize, perlinResult * 0.1 - SquareSize, z - SquareSize);
				glm::vec3 g(x + SquareSize, perlinResult * 0.1 - SquareSize, z - SquareSize);
				glm::vec3 h(x + SquareSize, perlinResult * 0.1 - SquareSize , z + SquareSize);
				
				mesh.addCube(a, b, c, d, e, f, g, h, colour);
			}
			else if (perlinResult > lastPerlinResult)
			{
				glm::vec3 a(x - SquareSize, perlinResult + SquareSize, z + SquareSize);
				glm::vec3 b(x + SquareSize, perlinResult + SquareSize, z + SquareSize);
				glm::vec3 c(x + SquareSize, perlinResult + SquareSize, z - SquareSize);
				glm::vec3 d(x - SquareSize, perlinResult + SquareSize, z - SquareSize);
				glm::vec3 e(x - SquareSize, perlinResult - SquareSize, z + SquareSize);
				glm::vec3 f(x - SquareSize, perlinResult - SquareSize, z - SquareSize);
				glm::vec3 g(x + SquareSize, perlinResult - SquareSize, z - SquareSize);
				glm::vec3 h(x + SquareSize, perlinResult - SquareSize, z + SquareSize);

				mesh.addCube(a, b, c, d, e, f, g, h, colour);
			}

			lastPerlinResult = perlinResult;
		}
	}
}