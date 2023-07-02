#version 320 es

layout (location = 0) in vec4 position;

uniform mat4 txMat;

out vec2 texCoord;

void main() {
	gl_Position = txMat * position;
	texCoord = position.xy;
}
