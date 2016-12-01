#pragma once
class ObjLoader
{
public:
	/**
	Constructor & Destructor
	*/
	ObjLoader();
	~ObjLoader();

	/**
	Function to load an .OBJ file and show it
	on screen
	*/
	bool LoadObj(const char * path,
		std::vector < glm::vec3 > & out_vertices,
		std::vector < glm::vec2 > & out_uvs,
		std::vector < glm::vec3 > & out_normals);
};

