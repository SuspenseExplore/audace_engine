#version 320 es

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec3 a_tangent;
layout (location = 3) in vec3 a_TexCoord0;
layout (location = 4) in vec3 a_TexCoord1;

uniform mat4 worldMat[128];
uniform mat4 vpMat;

out vec3 fragPos;
out vec3 normal;
//out vec3 tangent;
out vec3 texCoord0;

void main() {
	gl_Position = vpMat * worldMat[gl_InstanceID] * a_position;
	fragPos = (worldMat[gl_InstanceID] * a_position).xyz;
	normal = normalize(mat3(transpose(inverse(worldMat[gl_InstanceID]))) * a_normal);
//	tangent = a_tangent;
	texCoord0 = a_TexCoord0;
}
