#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;
uniform vec4 color;

uniform float kernel[9];
uniform float offsetScale;

out vec4 fragColor;

const float offsetX = 1.0 / 1920.0;
const float offsetY = 1.0 / 1080.0;

vec2 offsets[9] = vec2[](
	vec2(-offsetX, offsetY),
	vec2(0.0, offsetY),
	vec2(offsetX, offsetY),
	vec2(-offsetX, 0.0),
	vec2(0.0, 0.0),
	vec2(offsetX, 0.0),
	vec2(-offsetX, -offsetY),
	vec2(0.0, -offsetY),
	vec2(offsetX, -offsetY)
);

void main() {
	vec4 smp[9];
	for (int i = 0; i < 9; i++)
	{
		smp[i] = texture(tex1, texCoord + (offsets[i] * offsetScale)) * color;
	}
	vec4 col = vec4(0.0);
	for (int i = 0; i < 9; i++)
	{
		col += smp[i] * kernel[i];
	}
	fragColor = col;
}
