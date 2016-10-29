#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;
uniform vec3 eyeDirection;
uniform float specularIntensity;

out vec4 fragmentColour;

void main()
{
	vec3 E = normalize(eyeDirection);
	vec3 R = (-lightDirection, normal);
	vec3 lightDirectionNorm = normalize(lightDirection);


	float cosAlpha = clamp( dot( E,R ), 0,1 );
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	float ambientIntensity  = 0.5;
	float lightIntensity = diffuseIntensity + pow(cosAlpha, specularIntensity) + ambientIntensity;

	//fragmentColour = vec4(colour, 1.0);
	fragmentColour = vec4(lightIntensity * colour, 1.0);      // * texture(textureSampler, uv);
}
