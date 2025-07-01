#version 320 es
precision mediump float;

struct Material {
	vec4 baseColorFactor;
	float metallicFactor;
	float roughnessFactor;
	vec3 emissiveFactor;
	sampler2D baseColorMap;
	sampler2D normalMap;
	sampler2D metallicMap;
	sampler2D roughnessMap;
	sampler2D occlusionMap;
	sampler2D emissiveMap;
};

uniform Material material;

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform vec4 ambientLight;

in vec3 texCoord;
in vec3 fragPos;
in vec3 tViewPos;
in PointLight tLight;

out vec4 fragColor;

const float PI = 3.14159265359;
const vec3 ZERO = vec3(0.0, 0.0, 0.0);
const vec3 ONE = vec3(1.0, 1.0, 1.0);

void main() {
	vec3 ambientColor = clamp(ambientLight.rgb * ambientLight.a, ZERO, ONE);
	vec3 baseColor = material.baseColorFactor.rgb * texture(material.baseColorMap, texCoord.xy).rgb * material.baseColorFactor.a;
	vec3 surfaceNormal = texture(material.normalMap, texCoord.xy).rgb * 2.0 - 1.0;
	float roughness = material.roughnessFactor * texture(material.roughnessMap, texCoord.xy).r;
	float metallic = material.metallicFactor * texture(material.metallicMap, texCoord.xy).b;
	vec3 resultAmbient = ambientColor;

//	float diffuseFactor = max(dot(surfaceNormal, dirLightDirection), 0.0);
//	vec3 dfDirLightColor = clamp(diffuseFactor * dirLightColor.a * dirLightColor.rgb, ZERO, ONE);
//	vec3 resultDirLight = clamp(ambientColor + dfDirLightColor, ZERO, ONE) * dfBaseColor;

	vec3 ptDirection = normalize(tLight.position - fragPos);
	float ptDiffuseFactor = max(dot(surfaceNormal, ptDirection), 0.0);
	vec3 dfPointLightColor = clamp(ptDiffuseFactor * tLight.intensity * tLight.color.rgb, ZERO, ONE);
	vec3 resultPtLight = clamp(ambientColor + dfPointLightColor, ZERO, ONE) * baseColor;

	vec3 viewDir = normalize(tViewPos - fragPos);
	vec3 reflectDir = reflect(-ptDirection, surfaceNormal);
	float spFactor = pow(max(0.0, dot(viewDir, reflectDir)), metallic * 128.0);
	vec3 spColor = clamp(tLight.color * spFactor * (1.0 - roughness), ZERO, ONE);
	vec3 resultSpec = clamp(ambientColor + dfPointLightColor + spColor, ZERO, ONE) * baseColor;

	vec3 emissiveColor = texture(material.emissiveMap, texCoord.xy).rgb * material.emissiveFactor;
	vec3 resultEm = clamp(ambientColor + dfPointLightColor + spColor + emissiveColor, ZERO, ONE) * baseColor;

	vec3 result = fragPos;
//	result = mix(result, baseColor, outMtlColor);
//	result = mix(result, surfaceNormal, outNormal);
//	result = mix(result, resultAmbient, outAmbient);
//	result = mix(result, resultDirLight, outDirLight);
//	result = mix(result, resultPtLight, outFull);
	fragColor = vec4(resultEm, 1.0);

//	vec3 lightColor = calcLightColor(0, surfaceNormal) + calcLightColor(1, surfaceNormal) + calcLightColor(2, surfaceNormal) + calcLightColor(3, surfaceNormal);
//	fragColor = vec4(clamp(ambient + lightColor, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)), 1.0);
}
