#version 320 es
precision mediump float;

struct Material {
	vec3 ambient;
	sampler2D ambientMap;
	vec3 diffuse;
	sampler2D diffuseMap;
	sampler2D normalMap;
	vec3 specular;
	sampler2D specularMap;
	vec3 emission;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform sampler2D tex1;
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
	return diffuse;
}

vec3 calcSpecular(int lightIndex, vec3 surfaceNormal) {
	vec3 lightDir = normalize(tangentLightPos[lightIndex] - tangentFragPos);
	vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), (1.0 - texture(material.specularMap, texCoord.xy).r) * 128.0);
	vec3 specular = light[lightIndex].color * spec * material.specular;
	return specular;
}

vec3 calcLightColor(int lightIndex, vec3 surfaceNormal) {
	return calcDiffuse(lightIndex, surfaceNormal) + calcSpecular(lightIndex, surfaceNormal);
}

void main() {
	vec3 ambient = (ambientLight.rgb * ambientLight.a) * material.ambient * texture(material.ambientMap, texCoord.xy).r;

	vec3 surfaceNormal = texture(material.normalMap, texCoord.xy).rgb;
	surfaceNormal = normalize(surfaceNormal * 2.0 - 1.0);
	vec3 lightColor = calcLightColor(0, surfaceNormal) + calcLightColor(1, surfaceNormal) + calcLightColor(2, surfaceNormal) + calcLightColor(3, surfaceNormal);

	fragColor = vec4(ambient + material.emission + lightColor, 1.0);
//	fragColor = vec4(surfaceNormal * 0.5 + 0.5, 1.0);
}
