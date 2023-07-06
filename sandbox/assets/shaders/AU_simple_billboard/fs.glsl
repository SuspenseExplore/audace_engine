#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;
uniform vec4 color;

out vec4 fragColor;

void main() {
	fragColor = texture(tex1, texCoord) * color;
}
