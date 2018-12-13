#pragma once

#include "glew.h"
#include "glm/glm.hpp"

#include <vector>

// Default vertex type
struct Vertex {
	union {
		struct { float x, y, z; };
		glm::vec3 position;
	};

	union {
		struct { float r, g, b; };
		glm::vec3 colour;
	};

	union {
		struct { float normalX, normalY, normalZ; };
		glm::vec3 normal;
	};
	
	union {
		struct { float u, v; };
		glm::vec2 uvs;
	};

	unsigned __int8 boneIndices[4];
	float boneWeights[4];
};

// Post-processing vertex type (2D normalized)
struct PostProcessVertex {
	glm::vec2 position;
};

// Custom vertex format used when creating vertex buffers
class VertexFormat {
public:
	struct VertexAttribute {
		GLenum type;  // The GL type of variable. This supports GLM vectors
		int offset;   // The offset of the variable in the struct
		int location; // The location of the matching variable in the shader
		int size;     // The number of attributes by OpenGL definition. Restricted to 1-4 because ¯\_(ツ)_/¯

		// Pointer to the previous vertex attribute
		VertexAttribute* previous;
	};

	VertexFormat() : vertexSize(0) {};

public:
	// Creates the vertex format from a list of struct member variables. Each var is treated assigned the consecutive location index.
	// An example usage of the function with a vertex struct called 'Vertex' could be: CreateFromStructVars(&Vertex::position, &Vertex::boneWeights[0], ...)
	// This is useful for quickly creating vertex formats with no implicit conversions or ambiguity between vectors and sets of floats
	template<typename VertexType, typename VarType, typename ...types>
	VertexFormat(VarType VertexType::* attribute, types... arguments) {
		CreateFromStructVars(attribute, arguments...);
	}

	// Deferred creation constructor: same behaviour as VertexFormat(VarType ...)
	template<typename VertexType, typename VarType, typename ...types>
	void CreateFromStructVars(VarType VertexType::* attribute, types... arguments);

public:
	// Getters
	int GetVertexSize() const { return vertexSize; }
	const std::vector<VertexAttribute>& GetAttributeList() const { return attributeList; };

private:
	// Iterates through the struct variables
	template<typename VertexType, typename VarType, typename ...types>
	void IterateStructVars(VertexAttribute* lastAttribute, VarType VertexType::* attribute, types... arguments);
	void IterateStructVars(VertexAttribute* lastAttribute);

	template<typename VertexType, size_t arraySize> void SetAttribInfo(VertexAttribute* attributeDef, unsigned char (VertexType::* attributePtr)[arraySize]) {
		attributeDef->type = GL_UNSIGNED_BYTE;
		attributeDef->size = arraySize;
	}

	template<typename VertexType, size_t arraySize> void SetAttribInfo(VertexAttribute* attributeDef, float (VertexType::* attributePtr)[arraySize]) {
		attributeDef->type = GL_FLOAT;
		attributeDef->size = arraySize;
	}

	template<typename VertexType> void SetAttribInfo(VertexAttribute* attributeDef, int VertexType::* attributePtr) {
		attributeDef->type = GL_INT;
		attributeDef->size = 1;
	}

	template<typename VertexType> void SetAttribInfo(VertexAttribute* attributeDef, unsigned char VertexType::* attributePtr) {
		attributeDef->type = GL_UNSIGNED_INT;
		attributeDef->size = 1;
	}

	template<typename VertexType> void SetAttribInfo(VertexAttribute* attributeDef, float VertexType::* attributePtr) {
		attributeDef->type = GL_FLOAT;
		attributeDef->size = 1;
	}

	template<typename VertexType> void SetAttribInfo(VertexAttribute* attributeDef, glm::vec3 VertexType::* attributePtr) {
		attributeDef->type = GL_FLOAT;
		attributeDef->size = 3;
	}

	template<typename VertexType> void SetAttribInfo(VertexAttribute* attributeDef, glm::vec2 VertexType::* attributePtr) {
		attributeDef->type = GL_FLOAT;
		attributeDef->size = 2;
	}

private:
	// A list of vertex attributes to pass to glVertexAttribPointer
	std::vector<VertexAttribute> attributeList;

	// The size of the vertex (= stride)
	int vertexSize;
};

template<typename VertexType, typename VarType, typename ...types> inline void VertexFormat::CreateFromStructVars(VarType VertexType::* attribute, types... arguments) {
	// Set the vertex size
	vertexSize = sizeof (VertexType);

	// Recursively continue until the last attribute is reached
	IterateStructVars(nullptr, attribute, arguments...);
}

template<typename VertexType, typename VarType, typename ...types> inline void VertexFormat::IterateStructVars(VertexAttribute* lastAttrib, VarType VertexType::* attribute, types... arguments) {
	VertexAttribute currentAttrib;

	// Fill in info for this current attribute
	currentAttrib.previous = lastAttrib;
	currentAttrib.location = lastAttrib ? lastAttrib->location + 1 : 0;
	currentAttrib.offset = (int)&(((VertexType*)nullptr)->*attribute);
	SetAttribInfo(&currentAttrib, attribute);

	attributeList.push_back(currentAttrib);

	// Recursively continue until the last attribute is reached
	IterateStructVars(&currentAttrib, arguments...);
}

inline void VertexFormat::IterateStructVars(VertexAttribute* lastAttrib) {
	printf("Done\n");
}