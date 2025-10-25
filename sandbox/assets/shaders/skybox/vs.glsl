#version 320 es

layout (location = 0) in vec4 position;

uniform mat4 worldMat;
uniform mat4 vpMat;

out vec3 texCoord;

void main() {
	vec4 p = vec4(position.xyz - 0.5, position.w);
	gl_Position = vpMat * worldMat * p;
	texCoord = p.xyz;
}
