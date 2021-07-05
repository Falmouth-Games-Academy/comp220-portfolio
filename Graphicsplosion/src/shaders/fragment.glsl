#version 330 core

out vec4 color;

in vec3 fragColour;
in vec2 fragUv;
in vec3 fragNormal;
in vec3 fragShadowPosition;
in vec3 viewDirection;

uniform sampler2D textureSampler;
uniform sampler2D shadowSampler;
uniform sampler2D normalSampler;

uniform int isShadowEnabled;
uniform int isNormalMapEnabled;

uniform vec3 directionalLightDirection;

const float specularIntensity = 0.4f;
const float specularPower = 7.0f;

void main()
{
	// Do basic diffuse texturing with vertex colours
	color = vec4(fragColour, 1.0f) * texture(textureSampler, fragUv).xyzw;
	
	// Apply normal map if it's enabled (disabled due to time constraints)
	vec3 currentFragNormal = fragNormal;
	
	/*if (isNormalMapEnabled == 1) {
		currentFragNormal = texture(normalSampler, fragUv).xyz * 2.0f - 1.0f; // none of this will work without tangents!
	}*/
	
	// Add specular lighting
	vec3 halfWay = normalize(directionalLightDirection - viewDirection);
	float nDoth = pow(clamp(-dot(currentFragNormal, halfWay), 0.0f, 1.0f), specularPower);

	color.xyz += vec3(specularIntensity, specularIntensity, specularIntensity) * nDoth;
	
	// Add shadowing
	if (isShadowEnabled == 1) {
		const float minBias = 0.02f, maxBias = 0.04f;
		const float shadeBrightness = 0.5f;
		const float fadeoutBorder = 0.08f;

		if (lessThanEqual(fragShadowPosition.xy, vec2(1.0f)).xy == bvec2(true, true) && greaterThanEqual(fragShadowPosition.xy, vec2(0.0f)) == bvec2(true, true) 
		 && texture(shadowSampler, fragShadowPosition.xy).x < 0.9999f) {
			float lightMultiplier = mix(shadeBrightness, 1.0f, clamp((texture(shadowSampler, fragShadowPosition.xy).x - fragShadowPosition.z + minBias) / (maxBias - minBias), 0.0f, 1.0f));

			color.xyz = mix(color.xyz, color.xyz * lightMultiplier, clamp(min(min(fragShadowPosition.x, fragShadowPosition.y), min(1.0f - fragShadowPosition.x, 1.0f - fragShadowPosition.y)) / fadeoutBorder, 0.0f, 1.0f));
		}
	}

	// Cull invisible pixels from the depth buffer
	if (color.a <= 0.1f) {
		discard;
	}
}