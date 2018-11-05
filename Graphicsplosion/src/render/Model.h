#pragma once
#include "Renderer.h"

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
	
	// Cached buffers
	mutable class VertexBuffer vertexBuffer;
	mutable class IndexBuffer indexBuffer;
	mutable bool areBuffersCreated;
};