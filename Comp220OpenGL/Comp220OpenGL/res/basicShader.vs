#version 120

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

varying vec2 texCoord0;
varying vec3 normal0;
							//a uniform variable is a variable that can be set by the CPU
uniform mat4 transform;

							//lambirtion lighting- done per pixel
	
void main()
{
							//takes what ever matrix we have set and it apply it to our position 
	gl_Position =transform * vec4(position, 1.0); 	//(number is the amount of translation times)
	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal,0.0)).xyz;
}