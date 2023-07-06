#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;
uniform vec3 color;

out vec4 fragColor;

void main() {
	vec4 c = texture(tex1, texCoord);
	fragColor = vec4(color, c.a);
}
