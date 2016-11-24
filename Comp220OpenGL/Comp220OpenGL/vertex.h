#ifndef VERTEX_H
#define VERTEX_H

#include "math3d.h"

struct vertex
{
	Vector3f pos;
	Vector2f texCoord;
	Vector3f normal;
	Vector3f tangent;

	vertex(Vector3f pos, Vector2f texCoord = Vector2f(0,0), Vector3f normal = Vector3f(0,0,0), Vector3f tangent = Vector3f(0, 0, 0)):
		pos(pos),
		texCoord(texCoord),
		normal(normal),
		tangent(tangent) {}
			
};

#endif // !VERTEX_H
