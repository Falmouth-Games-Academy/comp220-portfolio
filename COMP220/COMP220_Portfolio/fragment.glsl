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
	vec3 normalizedNormal = normalize( normal );
    vec3 normalizedlightDirection = normalize( lightDirection );
	float distance = length(lightDirection - normalizedNormal);

	// Ambient lighting
	vec3 MaterialAmbientColor = ambientLightColour * colour;
    float cosTheta = dot(normalizedNormal, normalizedlightDirection);

	// Diffuse Lighting
	vec3 normalizedCamera = normalize(cameraSpace);
	vec3 reflected = reflect(-lightDirection, normal);
	vec3 lightDirectionNorm = normalize(lightDirection);
	vec3 diffuseLighting = colour  * mainLightColour * cosTheta / (distance * distance) ;

	// Specular Lighting
	float cosAlpha = clamp(dot(normalizedCamera,reflected), 0, 1);
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	float lightIntensity = diffuseIntensity + pow(cosAlpha, 5);
	vec3 specularLighting = lightIntensity  * colour;
	
	fragmentColour = vec4(MaterialAmbientColor + diffuseLighting + specularLighting, 1.0) * texture(textureSampler, uv);

}