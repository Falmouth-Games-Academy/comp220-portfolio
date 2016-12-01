#pragma once
class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	//! Generate noise from seed
	void GenerateNoise(unsigned int seed);

	//! Generate noise
	void GeneratePerlinNoise();

	//! Stores seed value
	int seed;

	std::vector<int> p;
	
	//! Noise function
	double noise(double x, double y, double z);

	//! For fade function
	double fade(double t);

	//! For linear interpolation
	double lerp(double t, double a, double b);

	//! For gradient function
	double grad(int hash, double x, double y, double z);
};

