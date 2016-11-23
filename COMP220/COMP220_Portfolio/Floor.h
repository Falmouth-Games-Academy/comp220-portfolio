#pragma once
#include"Mesh.h"
#include"Texture.h"
class Floor
{
public:
	Floor();
	Floor(int floorYPosition, int initialFloorSize, const std::string& floorTexture);
	~Floor();

	Texture texture;
	Mesh mesh;

	int getFloorY() { return floorY; }
	int getFloorSize() { return floorSize; }

private:
	const float floorY = -2;
	const float floorSize = 10;
};

