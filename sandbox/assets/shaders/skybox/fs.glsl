#version 320 es
precision mediump float;

in vec3 texCoord;
out vec4 fragColor;

uniform samplerCube tex1;
uniform vec4 color;

void main() {
	fragColor = texture(tex1, texCoord) * color;
}
