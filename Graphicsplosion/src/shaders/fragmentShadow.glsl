#version 330 core

layout(location = 0) out float vertexDepth;

void main() {
	vertexDepth = gl_FragCoord.z;
}