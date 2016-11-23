#include "stdafx.h"
#include "Voxel.h"


Voxel::Voxel()
{
}


Voxel::~Voxel()
{
}

Voxel::Voxel(int x, int z)
{
	setVoxelPosition(glm::vec3(x, 0, z));
}

void Voxel::placeVoxel(Mesh& meshTexture, glm::vec3 voxelPosition)
{
	setVoxelPosition(voxelPosition);

	glm::vec3 a(voxelPosition.x - 0.5, voxelPosition.y + 0.5, voxelPosition.z + 0.5);
	glm::vec3 b(voxelPosition.x + 0.5, voxelPosition.y + 0.5, voxelPosition.z + 0.5);
	glm::vec3 c(voxelPosition.x + 0.5, voxelPosition.y + 0.5, voxelPosition.z - 0.5);
	glm::vec3 d(voxelPosition.x - 0.5, voxelPosition.y + 0.5, voxelPosition.z - 0.5);
	glm::vec3 e(voxelPosition.x - 0.5, voxelPosition.y - 0.5, voxelPosition.z + 0.5);
	glm::vec3 f(voxelPosition.x - 0.5, voxelPosition.y - 0.5, voxelPosition.z - 0.5);
	glm::vec3 g(voxelPosition.x + 0.5, voxelPosition.y - 0.5, voxelPosition.z - 0.5);
	glm::vec3 h(voxelPosition.x + 0.5, voxelPosition.y - 0.5, voxelPosition.z + 0.5);
	glm::vec3 colour(1, 1, 1);
	meshTexture.addCube(a, b, c, d, e, f, g, h, colour);
}