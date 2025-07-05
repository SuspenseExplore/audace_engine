#version 320 es

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

uniform mat4 worldMat;
uniform mat4 vpMat;

out vec3 uColor;

void main() {
	gl_Position = vpMat * worldMat * position;
	uColor = color;
}
