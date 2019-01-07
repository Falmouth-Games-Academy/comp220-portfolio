#pragma once
#include "GenericBuffer.h"

// Vertex buffer
class VertexBuffer : public GenericBuffer {
public:
	// Constructs an empty vertex buffer
	VertexBuffer() : vaoName(0) {};

	// Creates a buffer from an optional initial vertex array
	VertexBuffer(class Renderer& renderer, const class VertexFormat& vertexFormat, const void* initialData = nullptr, int initialDataSize = 0) {
		Create(renderer, vertexFormat, initialData, initialDataSize);
	}

	~VertexBuffer() {
		Destroy();
	}

public:
	// Creates the buffer
	void Create(class Renderer& renderer, const class VertexFormat& vertexFormat, const void* initialData = nullptr, int initialDataSize = 0);
	// Destroys the buffer
	void Destroy() override;

	// Sets the buffer data to a new array of vertices
	void SetData(const void* arrayData, int size);

	GLuint GetVAO() const { return vaoName; }

private:
	// GL buffer for the VAO
	GLuint vaoName;
};