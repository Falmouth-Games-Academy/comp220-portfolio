#version 120

varying vec2 texCoord0;
varying vec3 normal0;

							//when doing something more complex take it in as a uniform so you can specify what direction it is
uniform sampler2D diffuse;


void main()
{
	gl_FragColor = texture2D(diffuse, texCoord0)			//clamp so we dont have neggative lighting or too bright
		* clamp(dot(-vec3(0,0,1), normal0),0.4,0.6);  		//angle between them,times the amount of brightness
}								