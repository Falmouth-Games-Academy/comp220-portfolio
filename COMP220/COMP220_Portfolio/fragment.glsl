#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;

uniform sampler2D textureSampler;
uniform vec3 lightDirection;
uniform vec3 cameraSpace;
uniform vec3 ambientLightColour;
uniform vec3 mainLightColour;

out vec4 fragmentColour;

void main()
{
	vec3 normalized = normalize(normal);
	vec3 normalizedLightDirection = normalize(lightDirection);
	vec3 normalizedCamerSpace = normalize(cameraSpace);
	vec3 R = reflect(-lightDirection, normal);
	

	//Diffuse
	vec3 n = normalize( normal );
    vec3 l = normalize( lightDirection );
	float cosTheta = clamp(dot(n,l),0 ,1);
	vec3 diffuseLighting = mainLightColour * cosTheta;

	// Ambient 
	vec3 ambientLighting = ambientLightColour * diffuseLighting;

	float distance = length(lightDirection - n);

	vec3 lighting = diffuseLighting + ambientLighting;

	fragmentColour = vec4(lighting * colour, 1.0) * texture(textureSampler, uv);

}