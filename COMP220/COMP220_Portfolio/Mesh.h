#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec3& colour,
		const glm::vec2& t1, const glm::vec2& t2, const glm::vec2& t3);
	void addSquare(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d,
		const glm::vec3& colour,
		float u1, float u2, float v1, float v2);

	void addCircle(const glm::vec3& centre, float radius, int numPoints,
		const glm::vec3& colour);

	bool loadOBJ(const char * path,
		std::vector < glm::vec3 > & outVertices, std::vector < glm::vec2 > & outUvs, std::vector < glm::vec3 > & outNormals);



	void createBuffers(std::vector< glm::vec3 >vertices, std::vector< glm::vec3 >normals, std::vector< glm::vec2 >uvs);
	void draw();

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temporaryVertices;
	std::vector< glm::vec2 > temporaryUvs;
	std::vector< glm::vec3 > temporaryNormals;
	

private:
	

	std::vector<glm::vec3> m_vertexPositions, m_vertexColours;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0;
	GLuint vertexbuffer = 0, uvbuffer = 0, normalbuffer = 0;
};
