#include "stdafx.h"
#include "PerlinNoise.h"


PerlinNoise::PerlinNoise(float xMaxIn, float yMaxIn)
{
	xMax = xMaxIn;
	yMax = yMaxIn;
}


PerlinNoise::~PerlinNoise()
{
}

// Function to linearly interpolate between a0 and a1
// Weight w should be in the range [0.0, 1.0]
float PerlinNoise::lerp(float a0, float a1, float w)
{
	return (1.0 - w)*a0 + w*a1;
}

// Computes the dot product of the distance and gradient vectors.
float PerlinNoise::dotGridGradient(int ix, int iy, float x, float y)
{
	// Precomputed (or otherwise) gradient vectors at each grid node
	extern float Gradient[yMax][xMax][2];

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx*Gradient[iy][ix][0] + dy*Gradient[iy][ix][1]);
}

// Compute Perlin noise at coordinates x, y
float PerlinNoise::perlin(float x, float y)
{
	// Determine grid cell coordinates
	int x0 = (x > 0.0 ? (int)x : (int)x - 1);
	int x1 = x0 + 1;
	int y0 = (y > 0.0 ? (int)y : (int)y - 1);
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;
	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = lerp(n0, n1, sx);
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = lerp(n0, n1, sx);
	value = lerp(ix0, ix1, sy);

	return value;
}

/*

Wikipedia example

// Function to linearly interpolate between a0 and a1
// Weight w should be in the range [0.0, 1.0]
function lerp(float a0, float a1, float w) {
	return (1.0 - w)*a0 + w*a1;
}

// Computes the dot product of the distance and gradient vectors.
function dotGridGradient(int ix, int iy, float x, float y) {

	// Precomputed (or otherwise) gradient vectors at each grid node
	extern float Gradient[IYMAX][IXMAX][2];

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx*Gradient[iy][ix][0] + dy*Gradient[iy][ix][1]);
}

// Compute Perlin noise at coordinates x, y
function perlin(float x, float y) {

	// Determine grid cell coordinates
	int x0 = (x > 0.0 ? (int)x : (int)x - 1);
	int x1 = x0 + 1;
	int y0 = (y > 0.0 ? (int)y : (int)y - 1);
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Interpolate between grid point gradients
	float n0, n1, ix0, ix1, value;
	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = lerp(n0, n1, sx);
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = lerp(n0, n1, sx);
	value = lerp(ix0, ix1, sy);

	return value;
}*/