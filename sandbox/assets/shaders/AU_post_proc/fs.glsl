#version 320 es
precision mediump float;

in vec2 texCoord;
uniform sampler2D tex1;
uniform vec4 color;

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

// identity
float kernel[9] = float[](
	0.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
);

// sharpen
//float kernel[9] = float[](
//	-1.0, -1.0, -1.0,
//	-1.0, 9.0, -1.0,
//	-1.0, -1.0, -1.0
//);

// blur
//float kernel[9] = float[](
//	1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
//	2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
//	1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
//);

// edge detection
//float kernel[9] = float[](
//	1.0, 1.0, 1.0,
//	1.0, -8.0, 1.0,
//	1.0, 1.0, 1.0
//);

void main() {
	vec4 smp[9];
	for (int i = 0; i < 9; i++)
	{
		smp[i] = texture(tex1, texCoord + offsets[i]) * color;
	}
	vec4 col = vec4(0.0);
	for (int i = 0; i < 9; i++)
	{
		col += smp[i] * kernel[i];
	}
	fragColor = col;
}
