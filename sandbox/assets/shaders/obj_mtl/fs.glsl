#version 320 es
precision mediump float;

struct Material {
	vec3 diffuse;
	sampler2D diffuseMap;
};

uniform Material material;

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};

// [r,b,g,a] == [color, intensity]
uniform vec4 ambientLight;

// a directional light.  [r,g,b] is the color, a is the intensity
uniform vec4 dirLightColor;
uniform vec3 dirLightDirection;

uniform PointLight light[4];

uniform float outPosition;
uniform float outMtlColor;
uniform float outNormal;
uniform float outAmbient;
uniform float outDirLight;
uniform float outFull;

in vec3 texCoord;
in vec3 tangentViewPos;
in vec3 fragPos;
in vec3 surfaceNormal;
in vec3 tangentFragPos;
in vec3 tangentLightPos[4];

out vec4 fragColor;

vec3 ZERO = vec3(0.0, 0.0, 0.0);
vec3 ONE = vec3(1.0, 1.0, 1.0);

vec3 calcDiffuse(int lightIndex, vec3 surfaceNormal) {
	vec3 lightDir = normalize(tangentLightPos[lightIndex] - tangentFragPos);
	float diffuseIntensity = max(0.0, dot(surfaceNormal, lightDir)) * light[lightIndex].intensity;
	vec3 diffuse = light[lightIndex].color * material.diffuse * texture(material.diffuseMap, texCoord.xy).rgb * diffuseIntensity;
	return clamp(diffuse, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
}

vec3 calcLightColor(int lightIndex, vec3 surfaceNormal) {
	return calcDiffuse(lightIndex, surfaceNormal);
}

void main() {
	vec3 ambientLight = clamp(ambientLight.rgb * ambientLight.a, ZERO, ONE);
	vec3 dfBaseColor = material.diffuse;
	vec3 resultAmbient = ambientLight * dfBaseColor;

	float diffuseFactor = max(dot(surfaceNormal, dirLightDirection), 0.0);
	vec3 dfDirLightColor = clamp(diffuseFactor * dirLightColor.a * dirLightColor.rgb, ZERO, ONE);
	vec3 resultDirLight = clamp(ambientLight + dfDirLightColor, ZERO, ONE) * dfBaseColor;

	vec3 ptDirection = normalize(light[0].position - fragPos);
	float ptDiffuseFactor = max(dot(surfaceNormal, ptDirection), 0.0);
	vec3 dfPointLightColor = clamp(ptDiffuseFactor * light[0].intensity * light[0].color.rgb, ZERO, ONE);
	vec3 resultPtLight = clamp(ambientLight + dfDirLightColor + dfPointLightColor, ZERO, ONE) * dfBaseColor;

	vec3 result = fragPos;
	result = mix(result, material.diffuse, outMtlColor);
	result = mix(result, surfaceNormal, outNormal);
	result = mix(result, resultAmbient, outAmbient);
	result = mix(result, resultDirLight, outDirLight);
	result = mix(result, resultPtLight, outFull);
	fragColor = vec4(result, 1.0);

//	vec3 lightColor = calcLightColor(0, surfaceNormal) + calcLightColor(1, surfaceNormal) + calcLightColor(2, surfaceNormal) + calcLightColor(3, surfaceNormal);
//	fragColor = vec4(clamp(ambient + lightColor, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)), 1.0);
}
