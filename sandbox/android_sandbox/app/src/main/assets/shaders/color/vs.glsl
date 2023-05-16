#version 320 es

layout (location = 0) in vec4 position;

out vec2 texCoord;

void main() {
	gl_Position = position;
	texCoord = position.xy;
}
