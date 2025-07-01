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

uniform vec3 viewPos;
uniform vec4 ambientLight;
uniform PointLight ptLight[4];

in vec3 vNormal;
in vec3 texCoord;
in vec3 fragPos;
in mat3 tbnMat;

out vec4 fragColor;

vec4 ZERO = vec4(0.0, 0.0, 0.0, 0.0);
vec4 ONE = vec4(1.0, 1.0, 1.0, 1.0);

vec3 ambientColor(vec3 baseColor)
{
	return clamp(ambientLight.a * ambientLight.rgb * baseColor * texture(material.occlusionMap, texCoord.xy).r, ZERO.rgb, ONE.rgb);
}

vec3 diffuseColor(vec3 normal, PointLight light)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(0.0, dot(normal, lightDir));
	vec3 r = diff * light.color * light.intensity;
	return clamp(r, ZERO.xyz, ONE.xyz);
}

vec3 specularColor(vec3 normal, PointLight light)
{
	float metallic = 256.0 * material.metallicFactor * texture(material.metallicMap, texCoord.xy).b;
	float roughness = 1.0 - (material.roughnessFactor * texture(material.roughnessMap, texCoord.xy).g);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), metallic);
	vec3 r = roughness * spec * light.color * light.intensity;
	float dist = length(light.position - fragPos);
	return clamp(r * 0.5, ZERO.xyz, ONE.xyz);
}

vec3 emissiveColor()
{
	return material.emissiveFactor * texture(material.emissiveMap, texCoord.xy).rgb;
}

void main() {
	vec3 normal = texture(material.normalMap, texCoord.xy).xyz;
	normal = normal * 2.0 - 1.0;
	normal = normalize(tbnMat * normal);
	vec3 baseColor = material.baseColorFactor.rgb * texture(material.baseColorMap, texCoord.xy).rgb;
	vec3 aColor = ambientColor(baseColor);
	vec3 dColor = diffuseColor(normal, ptLight[0]) * baseColor;
	vec3 sColor = specularColor(normal, ptLight[0]);
	vec3 eColor = emissiveColor();
	vec3 color = clamp(aColor + dColor + sColor + eColor, ZERO.xyz, ONE.xyz);
	fragColor = vec4(color, 1.0);
//	fragColor = texture(material.baseColorMap, texCoord.xy);
}
