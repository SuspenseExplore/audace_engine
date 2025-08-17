#version 320 es

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 texCoord0;
layout (location = 4) in vec4 texCoord1;

uniform mat4 worldMat[1];
uniform mat4 vpMat;

out vec3 values[5];

void main() {
	gl_Position = vpMat * worldMat[0] * position;
	values[0] = position.xyz;
	values[1] = normal.xyz;
	values[2] = tangent.xyz;
	values[3] = texCoord0.xyz;
	values[4] = texCoord1.xyz;
}
