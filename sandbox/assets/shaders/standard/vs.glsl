#version 320 es

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 inTexCoord;

uniform mat4 worldMat;
uniform mat4 vpMat;
uniform vec3 textureScale;

out vec3 outTexCoord;
out vec3 surfaceNormal;
out vec3 fragPos;

void main() {
	gl_Position = vpMat * worldMat * position;
	outTexCoord = inTexCoord / textureScale;
	surfaceNormal = normalize(mat3(transpose(inverse(worldMat))) * normal);
	fragPos = (worldMat * position).xyz;
}
