#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;

out vec4 fragColor;

void main() {
	fragColor = texture(tex1, texCoord);
}
