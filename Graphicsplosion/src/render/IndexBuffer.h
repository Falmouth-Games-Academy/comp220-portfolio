#pragma once
#include "GenericBuffer.h"

// Index buffer
class IndexBuffer : public GenericBuffer {
public:
	// Creates an empty index buffer (use SetData)
	IndexBuffer() = default;

	// Creates a buffer using an optional initial vertex array
	IndexBuffer(Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0) {
		Create(renderer, initialData, initialDataSize);
	}

	~IndexBuffer() {
		Destroy();
	}

public:
	// Sets the data to a new array of vertices
	void SetData(const void* arrayData, int size);
};
