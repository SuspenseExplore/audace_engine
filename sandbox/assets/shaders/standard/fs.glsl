#version 320 es
precision mediump float;

struct Material {
	vec3 ambient;
	sampler2D ambientMap;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	vec3 emission;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform Material material;
uniform Light light[4];

uniform sampler2D tex1;
uniform vec4 ambientLight;
uniform vec4 diffusePos;
uniform vec4 diffuseColor;
uniform vec3 viewPos;

in vec3 outTexCoord;
in vec3 surfaceNormal;
in vec3 fragPos;

out vec4 fragColor;

vec3 calcDiffuse(Light light) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseIntensity = max(0.0, dot(surfaceNormal, lightDir)) * light.intensity;
	vec3 diffuse = light.color * material.diffuse * texture(material.diffuseMap, outTexCoord.xy).rgb * diffuseIntensity;
	return diffuse;
}

vec3 calcSpecular(Light light) {
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.color * spec * material.specular;
	return specular;
}

vec3 calcLightColor(Light light) {
	return calcDiffuse(light) + calcSpecular(light);
}

void main() {
	vec3 ambient = (ambientLight.rgb * ambientLight.a) * material.ambient * texture(material.ambientMap, outTexCoord.xy).r;

	vec3 lightColor = calcLightColor(light[0]) + calcLightColor(light[1]) + calcLightColor(light[2]) + calcLightColor(light[3]);

	fragColor = vec4(ambient + material.emission + lightColor, 1.0);
}
