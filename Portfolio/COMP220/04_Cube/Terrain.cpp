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
	int x = 0;
	for (x; x < chunkSize; x++)
	{
		for (int z = 0; z < chunkSize; z++)
		{
			double perlinResult = perlinNoise.noise((x / noiseAmplification), (z / noiseAmplification), y);

			// Forward declarations
			double lastPerlinResult = perlinResult;
			glm::vec3 lastA(0,0,0);
			glm::vec3 lastB(0,0,0);
			glm::vec3 lastC(0,0,0);
			glm::vec3 lastD(0,0,0);
			glm::vec3 lastE(0,0,0);
			glm::vec3 lastF(0,0,0);
			glm::vec3 lastG(0,0,0);
			glm::vec3 lastH(0,0,0);
			//Normalize values
			perlinResult = (char)((perlinResult - noiseMin) * (255 / (noiseMax - noiseMin)));;

			// Cube Colour
			if (perlinResult > 0)
			{
				colour = glm::vec3(perlinResult / 100, perlinResult / 50, perlinResult / 700); // Grassy texture
			}
			else
			{
				colour = glm::vec3(-perlinResult / 7, -perlinResult / 10, perlinResult / 70);
			}


			// Reduce the incline of the slope if it is below 0
			if (perlinResult < lastPerlinResult )
			{
				
				glm::vec3 a(x - SquareSize, 0 + perlinResult * 0.1, z + SquareSize);
				glm::vec3 b(x + SquareSize, 0 + perlinResult * 0.1, z + SquareSize);
				glm::vec3 c(x + SquareSize, 0 + perlinResult * 0.1, z - SquareSize);
				glm::vec3 d(x - SquareSize, 0 + perlinResult * 0.1, z - SquareSize);
				glm::vec3 e(x - SquareSize, 0 + perlinResult * 0.1, z + SquareSize);
				glm::vec3 f(x - SquareSize, 0 + perlinResult * 0.1, z - SquareSize);
				glm::vec3 g(x + SquareSize, 0 + perlinResult * 0.1, z - SquareSize);
				glm::vec3 h(x + SquareSize, 0 + perlinResult * 0.1, z + SquareSize);

				mesh.addCube(a, b, c, d, e, f, g, h, colour);

				glm::vec3 lastA(a);
				glm::vec3 lastB(b);
				glm::vec3 lastC(c);
				glm::vec3 lastD(d);
				glm::vec3 lastE(e);
				glm::vec3 lastF(f);
				glm::vec3 lastG(g);
				glm::vec3 lastH(h);
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