#pragma once
#include "glew.h"

// lazy wrapper for unsafe GLuint type
enum GLResource : GLuint {
	GLRESOURCE_NULL = 0,
};

// 3D renderer wrapper for OpenGL (by default)
class Renderer {
public:
	Renderer();

public:
	GLResource LoadShaderFromSourceFile(const char* filename, GLenum glShaderType);
};