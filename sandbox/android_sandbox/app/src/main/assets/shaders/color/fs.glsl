#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;

out vec4 fragColor;

void main() {
	vec4 c = texture(tex1, texCoord);
	fragColor = vec4(1.0, 1.0, 1.0, c.a);
}
