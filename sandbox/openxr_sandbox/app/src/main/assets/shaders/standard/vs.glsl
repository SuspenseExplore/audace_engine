#version 320 es
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 worldMat;
uniform mat4 vpMat;
uniform vec2 textureScale;

out vec2 texCoord;
out vec3 surfaceNormal;
out vec3 fragPos;

void main() {
	gl_Position = vpMat * worldMat * position;
	texCoord = position.xy * textureScale;
	surfaceNormal = normalize(mat3(transpose(inverse(worldMat))) * normal);
	fragPos = (worldMat * position).xyz;
}
