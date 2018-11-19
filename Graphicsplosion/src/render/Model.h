#pragma once
#include "Renderer.h"

#include <vector>

#include <glm/glm.hpp>

struct AnimNode {
	// The target bone's name and pointer
	std::string targetName;
	const struct Bone* target;

	// Keyframes
	struct Keyframe {
		float time;

		union {
			glm::vec3 vec;
		};
	};

	// Keyframe lists for translation, rotation and scale
	std::vector<Keyframe> translation;
	std::vector<Keyframe> rotation;
	std::vector<Keyframe> scale;

	// The current index of each keyframe, blended
	float translationKeyframeIndex;
	float rotationKeyframeIndex;
	float scaleKeyframeIndex;
};

struct Anim {
	std::vector<AnimNode> nodes;
};


struct Bone {
	// The index of this bone relative to the model
	int index;

	// The name of this bone
	std::string name;

	// Matrix that transforms vertices from model space to bone space
	glm::mat4 bindPose;

	// List of attached vertex indices and their weights
	struct VertexWeight {
		int index;
		float weight;
	};

	std::vector<VertexWeight> vertexWeights;
};

class Model {
public:
	Model() : vertices(nullptr), numVertices(0), indices(nullptr), numIndices(0) {}

	Model(const char* filename) {
		Create(filename);
	}

	~Model() {
		Destroy();
	}

public:
	// Loads the mesh from a file
	bool Create(const char* filename);

	// Destroys the model and frees data
	void Destroy();

public:
	// Self-explanatory getters and setters. Urgh C# has opened my eyes... C++, there MUST be a better way!?
	const Vertex* GetVertices() const {
		return vertices;
	}

	int GetNumVertices() const {
		return numVertices;
	}

	const unsigned int* GetIndices() const {
		return indices;
	}

	int GetNumIndices() const {
		return numIndices;
	}

public:
	// Renders the model with the given renderer
	void Render(Renderer renderer);

private:
	// Array of the vertices in this mesh
	Vertex* vertices;
	unsigned int numVertices;

	// Array of the vertex indices in this mesh
	unsigned int* indices;
	unsigned int numIndices;
	
	// Array of the bones in this mesh
	std::vector<Bone> bones;

	// Array of the animations in this model
	std::vector<Anim> animations;

	// Cached buffers
	mutable class VertexBuffer vertexBuffer;
	mutable class IndexBuffer indexBuffer;
	mutable bool areBuffersCreated;
};