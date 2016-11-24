#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute vec3 tangent;

varying vec2 texCoord0;
varying mat3 tbnMatrix;
							//a uniform variable is a variable that can be set by the CPU
uniform mat4 transform;

							//lambirtion lighting- done per pixel
	
void main()
{
							//takes what ever matrix we have set and it apply it to our position 
	gl_Position =transform * vec4(position, 1.0); 	//(number is the amount of translation times)
	texCoord0 = texCoord;



	vec3 n = normalize((transform * vec4(normal,0.0)).xyz);
	vec3 t = normalize((transform * vec4(tangent,0.0)).xyz);

	t = normalize(t - dot(t,n) * n);

	vec3 biTangent = cross(t,n);

	tbnMatrix = mat3(t,biTangent, n);
}