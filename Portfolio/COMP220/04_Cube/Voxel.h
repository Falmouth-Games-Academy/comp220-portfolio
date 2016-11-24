#pragma once
#include "Mesh.h"

class Voxel
{
public:
	Voxel();
	~Voxel();

	Voxel(int x, int y, int z);

	bool isVoxel = true;

	float getVoxelSize() { return voxelSize; }
	glm::vec3 getvoxelPosition() { return voxelPosition;  }
	glm::vec3 setVoxelPosition(glm::vec3 newVoxelPosition) { return voxelPosition = newVoxelPosition; }
	
	void placeVoxel(Mesh& meshTexture, Mesh& snowTexture, glm::vec3 voxelPosition);

private:
	glm::vec3 voxelPosition;
	float voxelSize = 0.5;
};

