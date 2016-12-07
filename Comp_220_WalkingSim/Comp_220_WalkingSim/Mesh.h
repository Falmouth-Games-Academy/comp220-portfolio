#pragma once

#include "Vertex.h"

class Mesh
{
public:
	/**
	Constructor & Destructor
	*/
	Mesh();
	~Mesh();

	/**
	Function to add a triangle, it takes 3 floats to decide
	how big the triangle is to be
	*/
	void AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	/**
	Function to add a Square, it takes 4 floats to decide
	how big the triangle is to be
	*/
	void AddSquare(const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4);

	/**
	Function to add a Shpere, it takes 3 floats to decide
	the size, quality and colour
	*/
	void AddSphere(float radius, int quality, const glm::vec3& colour);

	/**
	Function to create the buffers for OpenGl so
	it know what to do with the information given to it
	*/
	void CreateBuffers();

	/**
	The draw function to make the final object appear on screen
	*/
	void Draw();

	/**
	Function to load an .OBJ file and show it 
	on screen
	*/
	bool LoadObj(
		const char * path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

	/**
	Function to add a Texture to an object
	*/
	void LoadTexture();

private:
	/**
	Setting up the vectors for the four variables and
	setting them to 0
	*/
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours, m_vertexNormals;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0, m_normalBuffer = 0;

	/**

	*/
	void AddVertex(const Vertex& vertex);

	/**
	Function to add vertices to a shpere
	Must be called before the buffers as they can nbot be
	added after
	*/
	Vertex CreateSphereVertex(float radius, float longitude, float latitude, const glm::vec3& colour);
};
