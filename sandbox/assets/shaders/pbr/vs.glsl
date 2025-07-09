#version 320 es
precision mediump float;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 inTexCoord0;

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};

uniform mat4 worldMat;
uniform mat4 vpMat;
uniform vec3 textureScale;
uniform vec3 viewPos;
uniform PointLight ptLight[4];
uniform vec3 dirLightDirection;

out vec3 texCoord;
out vec3 fragPos;
out vec3 tViewPos;
out PointLight tLight;
out vec3 tDirLightDir;

void main() {
	gl_Position = vpMat * worldMat * position;
//	texCoord = inTexCoord0 / textureScale;
	texCoord = inTexCoord0;

	vec3 n = normalize(mat3(worldMat) * normal);
	vec3 t = normalize(mat3(worldMat) * tangent);
	t = normalize(t - dot(t, n) * n);
	vec3 binormal = cross(n, t);
	mat3 tbnMat = transpose(mat3(t, binormal, n));

	fragPos = tbnMat * (worldMat * position).xyz;
	tViewPos = tbnMat * viewPos;
	tLight.position = tbnMat * ptLight[0].position;
	tLight.color = ptLight[0].color;
	tLight.intensity = ptLight[0].intensity;
	tDirLightDir = tbnMat * dirLightDirection;
}
