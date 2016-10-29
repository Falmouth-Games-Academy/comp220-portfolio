#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;

out vec4 fragmentColour;

void main()
{
	vec3 lightDirectionNorm = normalize(lightDirection);
	vec3 cameraDirectionNorm = normalize(gl_Position);

	float diffuseIntensity = dot(normal, lightDirectionNorm);
	vec3 ambientColor = vec3(0.3, 0.3, 0.3);
	vec3 lighting = ambientColor * diffuseIntensity;
	fragmentColour = vec4(lighting * colour, 1.0) * texture(textureSampler, uv);

}