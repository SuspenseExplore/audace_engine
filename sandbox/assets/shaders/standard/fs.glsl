#version 320 es
precision mediump float;

struct Material{
	sampler2D baseColorMap;
	vec4 baseColorFactor;
};
uniform Material material;

//struct PointLight {
//	vec3 position;
//	vec3 color;
//	float intensity;
//};

struct DirLight {
	vec3 direction;
	vec4 color;
};

//struct SpotLight {
//	vec3 position;
//	vec3 direction;
//	vec4 color;
//	float innerAngle;
//	float outerAngle;
//};

uniform vec4 ambientLight;
//uniform PointLight ptLight;
uniform DirLight dirLight;
//uniform SpotLight spotLight;

in vec3 fragPos;
in vec3 normal;
in vec3 texCoord0;

out vec4 fragColor;

//const float PI = 3.14159265359;
const vec4 ZERO = vec4(0.0, 0.0, 0.0, 0.0);
const vec4 ONE = vec4(1.0, 1.0, 1.0, 1.0);

vec3 getAmbientLight()
{
	// the light's alpha channel is intensity
	vec4 lightColor = clamp(ambientLight, ZERO, ONE);
	return lightColor.rgb * lightColor.a;
}

//vec3 getPointLightDiffuse(PointLight light)
//{
//	vec3 lightColor = clamp(light.color, ZERO.rgb, ONE.rgb);
//	vec3 lightDir = normalize(light.position - fragPos);
//	float diff = max(0.0, dot(normal, lightDir));
//	return diff * lightColor * light.intensity;
//}

vec3 getDirLightDiffuse(DirLight light)
{
	vec3 lightColor = clamp(light.color.rgb, ZERO.rgb, ONE.rgb);
	float diff = max(0.0, dot(normal, light.direction));
	return diff * lightColor * light.color.a;
}

void main() {
	vec3 baseColor = texture(material.baseColorMap, texCoord0.xy).rgb * material.baseColorFactor.rgb;
	vec3 lightColor = (getAmbientLight() + getDirLightDiffuse(dirLight));
	vec3 color = clamp(baseColor * lightColor, ZERO.rgb, ONE.rgb);
	fragColor = vec4(color, 1.0);
}
