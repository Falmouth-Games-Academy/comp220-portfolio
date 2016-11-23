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

	void createBuffers();
	void draw();

	bool LoadObj(
		const char * path,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals);

	void Load();

private:
	std::vector<glm::vec3> m_vertexPositions, m_vertexColours;
	std::vector<glm::vec2> m_vertexUVs;
	GLuint m_positionBuffer = 0, m_colourBuffer = 0, m_uvBuffer = 0;
};
