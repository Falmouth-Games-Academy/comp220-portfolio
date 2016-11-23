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

	int getY() { return y; }
	int getFloorSize() { return floorSize; }

private:
	const float y = -2;
	const float floorSize = 10;
};

