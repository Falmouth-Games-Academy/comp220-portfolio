#pragma once
class PerlinNoise
{
public:
	PerlinNoise(float xMaxIn, float yMaxIn);
	~PerlinNoise();

	// Compute Perlin noise at coordinates x, y
	float perlin(float x, float y);

private:
	// Computes the dot product of the distance and gradient vectors.
	float dotGridGradient(int ix, int iy, float x, float y);

	// Function to linearly interpolate between a0 and a1
	// Weight w should be in the range [0.0, 1.0]
	float lerp(float a0, float a1, float w);

	// stores the gradient vectors
	float xMax, yMax;
};
