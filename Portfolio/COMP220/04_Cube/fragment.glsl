#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;
uniform vec3 eyeDirection;
uniform float specularIntensity;
uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform float LightPower;
uniform vec3 lightPos;

// TODO: calculate distance


out vec4 fragmentColour;

void main()
{
	// Calculate Ambient
	vec3 MaterialAmbientColor = ObjectColor * colour;
    vec3 n = normalize( normal );
    vec3 l = normalize( lightDirection );

    float cosTheta = dot(n,l);

	// Calculate Diffuse
	vec3 E = normalize(eyeDirection);
	vec3 R = (-lightDirection, normal);
	vec3 lightDirectionNorm = normalize(lightDirection);

	// Calculate Specular
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	float lightIntensity = diffuseIntensity + pow(cosAlpha, specularIntensity);

	// Calculate Attenuation
	float distance = length(lightPos - n);
	
	fragmentColour = vec4(


	// Ambient
	MaterialAmbientColor + 

	// Diffuse
	colour * LightPower * LightColor * cosTheta / (distance * distance) + 

	// Specular
	lightIntensity * ObjectColor * LightPower, 1.0) * texture(textureSampler, uv);
	//fragmentColour = vec4(colour, 1.0);
}
