#pragma once

class Voxel
{
public:
	Voxel();
	~Voxel();

	float getVoxelSize() { return voxelSize; }
	glm::vec3 getvoxelPosition() { return voxelPosition;  }

	glm::vec3 setVoxelPosition(glm::vec3 newVoxelPosition) { return voxelPosition = newVoxelPosition; }
		

private:
	glm::vec3 voxelPosition;
	float voxelSize = 0.5;
};

