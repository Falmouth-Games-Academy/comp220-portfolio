#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;

uniform sampler2D textureSampler;
uniform vec3 lightDirection;
uniform vec3 cameraSpace;

out vec4 fragmentColour;

void main()
{
	vec3 cameraDirectionNorm = normalize(cameraSpace);
	vec3 lightDirectionNorm = normalize(lightDirection);

	vec3 reflection = reflect(-lightDirectionNorm, cameraDirectionNorm);

	float specularIntensity = clamp( dot( cameraDirectionNorm, reflection), 0,1 );
	float diffuseIntensity = clamp(dot(normal, lightDirectionNorm), 0,1);

	vec3 ambientColor = vec3(0.3, 0.3, 0.3); // change to uniform

	vec3 lighting = ambientColor + diffuseIntensity;

	fragmentColour = vec4(lighting * colour, 1.0) * texture(textureSampler, uv);

}