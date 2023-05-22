#version 320 es
precision mediump float;

uniform sampler2D tex1;
in vec2 texCoord;

out vec4 fragColor;

void main() {
	fragColor = texture(tex1, texCoord);
}
