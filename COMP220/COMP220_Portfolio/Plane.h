#pragma once
#include"Mesh.h"
#include"Texture.h"
//! Plane class
/*!
	Class for creating a plane 
*/
class Plane
{
public:
	//! Constructor
	Plane();
	//! Alternate constructor
	/*!
		Takes in the position, size and texture location for the plane
	*/
	Plane(int floorYPosition, int initialFloorSize, const std::string& floorTexture);
	//! Destructor
	~Plane();

	//! Particle texture
	Texture texture;
	//! Particle mesh
	Mesh mesh;

	//!  returns the in Y value
	int getY() { return y; }
	//!  returns the in floorSize value
	int getFloorSize() { return floorSize; }

private:
	//! int value of the Y position
	float y = -2;
	//! int value of the floorSize
	float floorSize = 10;
};

