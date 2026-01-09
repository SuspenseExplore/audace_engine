#version 320 es

layout (location = 0) in vec4 position;

uniform vec4 offset;
uniform mat4 worldMat[128];
uniform mat4 vpMat;

out vec2 texCoord;

void main() {
	gl_Position = vpMat * worldMat[gl_InstanceID] * (position + offset);
	texCoord = position.xy;
}
