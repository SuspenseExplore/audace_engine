#version 320 es
precision mediump float;

struct Material {
	vec3 diffuse;
	sampler2D diffuseMap;
};

uniform Material material;

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform vec4 ambientLight;
uniform vec4 diffusePos;
uniform vec4 diffuseColor;
uniform Light light[4];

in vec3 texCoord;
in vec3 tangentViewPos;
in vec3 fragPos;
in vec3 tangentFragPos;
in vec3 tangentLightPos[4];

out vec4 fragColor;

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
	vec3 ambient = (ambientLight.rgb * ambientLight.a) * material.diffuse;

	vec3 surfaceNormal = vec3(0.0, 0.0, 1.0);
	vec3 lightColor = calcLightColor(0, surfaceNormal) + calcLightColor(1, surfaceNormal) + calcLightColor(2, surfaceNormal) + calcLightColor(3, surfaceNormal);

	fragColor = vec4(clamp(ambient + lightColor, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)), 1.0);
}
