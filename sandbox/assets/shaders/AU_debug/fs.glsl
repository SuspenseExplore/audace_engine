#version 320 es
precision mediump float;

in vec3 values[5];
out vec4 fragColor;

uniform int outIndex;

void main() {
	fragColor = vec4(values[outIndex], 1.0);
}
