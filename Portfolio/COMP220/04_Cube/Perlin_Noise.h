#pragma once
class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	void GenerateNoise(unsigned int seed);
	void GenerateNoise();
	int seed;

	std::vector<int> p;
	
	double noise(double x, double y, double z);

	// For fade function
	double fade(double t);

	// For linear interpolation
	double lerp(double t, double a, double b);

	// For gradient function
	double grad(int hash, double x, double y, double z);
};

