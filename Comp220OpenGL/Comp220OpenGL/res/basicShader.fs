#version 120

varying vec2 texCoord0;
varying vec3 normal0;
varying mat3 tbnMatrix;
							//when doing something more complex take it in as a uniform so you can specify what direction it is
uniform sampler2D diffuse;
uniform sampler2D normalMap;

void main()
{
	vec3 normal = normalize(tbnMatrix * (255.0/128.0 * texture2D(normalMap,texCoord0.xy).xyz - 1));
	gl_FragColor = texture2D(diffuse, texCoord0)			//clamp so we dont have neggative lighting or too bright
		* clamp(dot(-vec3(0,0,1), normal0),0.0,1.0);  		//angle between them,times the amount of brightness
}								