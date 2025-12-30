#version 320 es

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 inTexCoord0;

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};
struct DirLight {
	vec3 direction;
	vec4 color;
};
struct SpotLight {
	vec3 position;
	vec3 direction;
	vec4 color;
	float innerAngle;
	float outerAngle;
};

uniform mat4 worldMat[128];
uniform mat4 vpMat;
uniform vec3 textureScale;
uniform vec3 viewPos;
uniform PointLight ptLight;
uniform DirLight dirLight;
uniform SpotLight spotLight;

out vec3 texCoord;
out vec3 fragPos;
out vec3 tViewPos;
out mat3 tbnMat;
out PointLight tLight;
out DirLight tDirLight;
out SpotLight tSpotLight;

void main() {
	gl_Position = vpMat * worldMat[gl_InstanceID] * position;
//	texCoord = inTexCoord0 / textureScale;
	texCoord = inTexCoord0;

	vec3 n = normalize(mat3(worldMat[gl_InstanceID]) * normal);
	vec3 t = normalize(mat3(worldMat[gl_InstanceID]) * tangent);
	t = normalize(t - dot(t, n) * n);
	vec3 binormal = cross(n, t);
	tbnMat = mat3(t, binormal, n);
	mat3 _tbnMat = transpose(tbnMat);

	fragPos = _tbnMat * (worldMat[gl_InstanceID] * position).xyz;
	tViewPos = _tbnMat * viewPos;

	tLight.position = _tbnMat * ptLight.position;
	tLight.color = ptLight.color;
	tLight.intensity = ptLight.intensity;

	tDirLight.direction = _tbnMat * dirLight.direction;
	tDirLight.color = dirLight.color;
	
	tSpotLight.position = _tbnMat * spotLight.position;
	tSpotLight.direction = _tbnMat * spotLight.direction;
	tSpotLight.color = spotLight.color;
	tSpotLight.innerAngle = spotLight.innerAngle;
	tSpotLight.outerAngle = spotLight.outerAngle;
}
