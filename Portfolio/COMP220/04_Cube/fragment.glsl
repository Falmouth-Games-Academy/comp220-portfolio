#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;
uniform vec3 eyeDirection;
uniform float specularIntensity;
uniform vec3 LightColor;
uniform float LightPower;

// TODO: calculate distance
uniform float distance;




out vec4 fragmentColour;

void main()
{

	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * colour;
	vec3 n = normalize( normal );
	vec3 l = normalize( lightDirection );

	float cosTheta = dot( n,l );




	vec3 E = normalize(eyeDirection);
	vec3 R = (-lightDirection, normal);
	vec3 lightDirectionNorm = normalize(lightDirection);


	float cosAlpha = clamp( dot( E,R ), 0,1 );
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	float lightIntensity = diffuseIntensity + pow(cosAlpha, specularIntensity);

	//fragmentColour = vec4(colour, 1.0);

	// Ambient
	fragmentColour = vec4(MaterialAmbientColor + 

	// Diffuse
	colour * LightColor * LightPower * cosTheta + 

	// Specular
	lightIntensity * LightColor * LightPower * pow(cosAlpha,5), 1.0
	);      // * texture(textureSampler, uv);
}
