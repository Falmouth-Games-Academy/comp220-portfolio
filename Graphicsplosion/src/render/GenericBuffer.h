#pragma once

#include "RenderResource.h"

// Generic buffer
class GenericBuffer {
public:
	// Default constructors or destructors
	GenericBuffer() : bufferName(0) {}

	~GenericBuffer() {
		// Cleanup if applicable
		Destroy();
	}

public:
	// Creates or destroys the buffer. Overridable. By default, uses renderer.CreateBuffer renderer.DestroyBuffer
	virtual void Create(class Renderer& renderer, const void* initialData = nullptr, int initialDataSize = 0);

	// Destroy may be called multiple times safely
	virtual void Destroy();

public:
	// Uploads new data to the buffer
	virtual void SetData(const void* arrayData, int size) = 0;

	// Returns the internal OpenGL buffer name
	GLuint GetBufferName() const { return bufferName; }

protected:
	class Renderer* renderer;

	GLuint bufferName;
};