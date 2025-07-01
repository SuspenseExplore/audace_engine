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
const vec4 ZERO = vec4(0.0, 0.0, 0.0, 0.0);
const vec4 ONE = vec4(1.0, 1.0, 1.0, 1.0);

float distributionGGX(vec3 n, vec3 h, float roughness)
{
	float a = roughness * roughness;
	float a2 = a*a;
	float n_dot_h = max(0.0, dot(n, h));
	float n_dot_h2 = n_dot_h * n_dot_h;

	float denom = (n_dot_h2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return a2 / denom;
}

float geomSchlickGGX(float n_dot_v, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return n_dot_v / (n_dot_v * (1.0 - k) + k);
}

float geomSmith(vec3 n, vec3 v, vec3 l, float roughness)
{
	float n_dot_v = max(0.0, dot(n, v));
	float n_dot_l = max(0.0, dot(n, l));
	float ggx2 = geomSchlickGGX(n_dot_v, roughness);
	float ggx1 = geomSchlickGGX(n_dot_l, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 f0)
{
	return f0 + (1.0 - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getBaseColor()
{
	return material.baseColorFactor.rgb * texture(material.baseColorMap, texCoord.xy).rgb;
}

float getMetallic()
{
	return material.metallicFactor * texture(material.metallicMap, texCoord.xy).b;
}

float getRoughness()
{
	return material.roughnessFactor * texture(material.roughnessMap, texCoord.xy).g;
}

float getOcclusion()
{
	return texture(material.occlusionMap, texCoord.xy).r;
}

vec3 getEmissive()
{
	return material.emissiveFactor * texture(material.emissiveMap, texCoord.xy).rgb;
}

void main() {
	vec3 normal = texture(material.normalMap, texCoord.xy).xyz;
	normal = normalize(normal * 2.0 - 1.0);

	vec3 baseColor = getBaseColor();
	float metallic = getMetallic();
	float roughness = getRoughness();
	float occlusion = getOcclusion();
	vec3 emissive = getEmissive();

	vec3 v = normalize(tViewPos - fragPos);
	vec3 f0 = vec3(0.04);
	f0 = mix(f0, baseColor, metallic);

	//reflectance
	vec3 lo = vec3(0.0);
	vec3 l = normalize(tLight.position - fragPos);
	vec3 h = normalize(v + l);
	float dist = length(tLight.position - fragPos);
	float attenuation = 1.0 / (dist * dist);
	vec3 radiance = tLight.color * tLight.intensity * attenuation;

	//cook-torrance brdf
	float ndf = distributionGGX(normal, h, roughness);
	float g = geomSmith(normal, v, l, roughness);
	vec3 f = fresnelSchlick(max(0.0, dot(h, v)), f0);

	vec3 ks = f;
	vec3 kd = ONE.xyz - ks;
	kd *= 1.0 - metallic;

	vec3 num = ndf * g * f;
	float denom = 4.0 * max(0.0, dot(normal, v)) * max(0.0, dot(normal, l)) + 0.0001;
	vec3 specular = num / denom;

	float n_dot_l = max(0.0, dot(normal, l));
	lo = (kd * baseColor / PI + specular) * radiance * n_dot_l;

	vec3 ambient = vec3(0.03) * baseColor * occlusion;
	vec3 color = clamp(ambient + lo + emissive, ZERO.xyz, ONE.xyz);

	color = color / (color + ONE.xyz);
	color = pow(color, vec3(1.0 / 2.2));

	fragColor = vec4(color, 1.0);
}
