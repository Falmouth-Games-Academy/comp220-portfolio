#include "stdafx.h"
#include "Terrain.h"


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
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

			//Normalize values
			perlinResult = (char)((perlinResult - noiseMin) * (255 / (noiseMax - noiseMin)));;

			// Cube Colour
			if (perlinResult > 0)
			{
				colour =

					//glm::vec3(sin(perlinResult), cos(perlinResult), tan(perlinResult));
					//glm::vec3(1, 0.5, 0), //Orange
					//glm::vec3(sin(perlinResult), cos(perlinResult), tan(perlinResult)), //Rainbow Red/White
					glm::vec3(perlinResult / 100, perlinResult / 50, perlinResult / 700); // Grassy texture
																						  //glm::vec3(sin(perlinResult) / 80 ,sin(perlinResult / 35), sin(perlinResult) / 100 ), // Grassy texture
																						  //glm::vec3(perlinResult / 10, perlinResult / 30, perlinResult / 75), // Grassy texture
			}
			else
			{
				colour = glm::vec3(-perlinResult / 7, -perlinResult / 10, -sin(perlinResult / 70));
			}

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
	}
}