#version 330 core

layout(location = 0) in vec2 vertexPos;

out vec2 uv;

void main()
{
	// Pass the position through to the fragment shader
	gl_Position = vec4(vertexPos, 0.0f, 1.0f);

	// Pass the normalised texture coords through
	uv = (gl_Position.xy + vec2(1.0f, 1.0f)) / 2.0f;
}