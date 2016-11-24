#include "stdafx.h"
#include "Voxel.h"


Voxel::Voxel()
{
}


Voxel::~Voxel()
{
}

Voxel::Voxel(int x, int y, int z)
{
	setVoxelPosition(glm::vec3(x, 0, z));
}

void Voxel::placeVoxel(Mesh& meshTexture, Mesh& snowTexture, glm::vec3 voxelPosition)
{
	setVoxelPosition(voxelPosition);
	
	if (voxelPosition.y < 0)
	{
		glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 colour(1, 1, 1);
		meshTexture.addCube(a, b, c, d, e, f, g, h, colour);
	}
	else
	{
		glm::vec3 a(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 b(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 c(voxelPosition.x + voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 d(voxelPosition.x - voxelSize, voxelPosition.y + voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 e(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 f(voxelPosition.x - voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 g(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z - voxelSize);
		glm::vec3 h(voxelPosition.x + voxelSize, voxelPosition.y - voxelSize, voxelPosition.z + voxelSize);
		glm::vec3 colour(1, 1, 1);
		snowTexture.addCube(a, b, c, d, e, f, g, h, colour);
	}
}