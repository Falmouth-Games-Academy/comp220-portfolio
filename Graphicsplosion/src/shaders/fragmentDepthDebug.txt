#version 330 core

out vec3 color;

uniform sampler2D textureSampler;

in vec2 fragUv;

void main() {
	color = texture(textureSampler, fragUv).xxx;
}