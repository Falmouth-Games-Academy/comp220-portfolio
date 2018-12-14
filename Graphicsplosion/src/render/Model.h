#pragma once
#include "Renderer.h"
#include "IndexBuffer.h"

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

struct AnimNode {
	// The target bone's name and pointer
	std::string targetName;
	const struct Bone* target;

	// Keyframes
	struct Keyframe {
		float time;

		union {
			glm::vec3 vec;
			glm::quat quat;
		};
	};

	// Keyframe lists for translation, rotation and scale
	std::vector<Keyframe> translation;
	std::vector<Keyframe> rotation;
	std::vector<Keyframe> scale;

	// The current index of each keyframe including blending
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

	// The parent bone
	const Bone* parent;

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
	bool Create(const char* filename, bool doPrecalculateInstances = false);

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

	// Returns a vector containing the file paths of every texture in this model
	const std::vector<std::string> GetTextureNames() const {
		return textureNames;
	}

public:
	// Tools
	// Finds a bone by its name
	Bone* FindBoneByName(std::string name) {
		for (Bone& b : bones) {
			if (b.name == name) {
				return &b;
			}
		}
		return nullptr;
	}

public:
	// Poses the model's bones according to animationTime
	void PoseBones(float animationTime);

	// Calculates the transformation matrices for each bone
	void CalculateBoneMatrices(glm::mat4 matrices[32]);

public:
	// Renders the model with the given renderer
	// 'textures' is an optional parameter for models with multiple textures. Any of these can be nullptr meaning the texture is not available.
	void Render(Renderer& renderer, const ShaderProgram& shaderProgram, const Texture* const* textures = nullptr);

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

	// File names of the textures in this model
	std::vector<std::string> textureNames;

	// Declares a section of the vertex buffer representing a new mesh with potentially new textures etc
	struct MeshSection {
		int startIndex;
		int numIndices;

		int textureIndex;
	};

	std::vector<MeshSection> meshSections;

	// Cached buffers
	mutable class VertexBuffer vertexBuffer;
	mutable class IndexBuffer indexBuffer;
	mutable bool areBuffersCreated;
};