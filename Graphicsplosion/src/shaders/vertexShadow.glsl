#version 330 core

layout(location = 0) in vec3 vertexPos;

out vec3 position;

uniform mat4 matShadowView;
uniform mat4 matWorld;

void main() {
	gl_Position = matShadowView * matWorld * vec4(vertexPos, 1.0f);
}