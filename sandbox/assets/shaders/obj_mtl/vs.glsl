#version 320 es
precision mediump float;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 inTexCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

struct Light {
	vec3 position;
	vec3 color;
	float intensity;
};
uniform Light light[4];

uniform mat4 worldMat;
uniform mat4 vpMat;
uniform vec3 textureScale;
uniform vec3 viewPos;

out vec3 texCoord;
out vec3 tangentViewPos;
out vec3 fragPos;
out vec3 tangentFragPos;
out vec3 tangentLightPos[4];

void main() {
	gl_Position = vpMat * worldMat * position;
	texCoord = inTexCoord / textureScale;
	fragPos = (worldMat * position).xyz;

	// calculate TBN matrix for normal mapping
	vec3 N = normalize(worldMat * vec4(normal, 0.0)).xyz;
	vec3 T = normalize(worldMat * vec4(tangent, 0.0)).xyz;
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));

	tangentViewPos = TBN * viewPos;
	tangentFragPos = TBN * fragPos;
	for (int i = 0; i < 4; i++)
	{
		tangentLightPos[i] = TBN * light[i].position;
	}
}
