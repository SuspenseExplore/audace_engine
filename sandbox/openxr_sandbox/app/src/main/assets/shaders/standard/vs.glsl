#version 320 es
layout (location = 0) in vec4 position;

uniform mat4 worldMat;
uniform mat4 vpMat;
uniform vec2 textureScale;
out vec2 texCoord;

void main() {
	gl_Position = vpMat * worldMat * position;
	texCoord = position.xy * textureScale;
}
