#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
in vec3 positionWorldspace;
in vec3 normalCameraspace;
in vec3 eyeDirection;
in vec3 lightDirectionCameraspace;

uniform sampler2D textureSampler;

uniform vec3 lightPosition;
uniform vec3 ambientLightColour;
uniform vec3 lightColour;
uniform float lightPower;

out vec4 fragmentColour;

void main()
{

	
	// Material properties
	vec3 materialDiffuseColor = texture( textureSampler, uv ).rgb;
	vec3 materialAmbientColor = vec3(0.1,0.1,0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length(lightPosition - positionWorldspace );

	
	vec3 n = normalize( normalCameraspace );
	
	vec3 l = normalize( lightDirectionCameraspace );
	
	float cosTheta = clamp( dot( n,l ), 0,1 );

	vec3 E = normalize(eyeDirection);
	
	vec3 R = reflect(-l,n);
	
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	fragmentColour = vec4(materialAmbientColor + materialDiffuseColor * lightColour * lightPower 
		* cosTheta / (distance*distance) + materialSpecularColor * lightColour * lightPower * pow(cosAlpha,5) / (distance*distance), 1.0);

}