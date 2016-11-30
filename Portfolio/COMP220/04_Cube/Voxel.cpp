#include "stdafx.h"
#include "Voxel.h"


Voxel::Voxel()
{
}


Voxel::~Voxel()
{
}

Voxel::Voxel(float x, float y, float z)
{
	setVoxelPosition(glm::vec3(x, y, z));
}