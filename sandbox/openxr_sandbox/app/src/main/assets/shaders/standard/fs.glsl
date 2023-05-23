#version 320 es
precision mediump float;

uniform sampler2D tex1;
uniform vec4 ambientLight;

in vec2 texCoord;

out vec4 fragColor;

void main() {
	vec4 objColor = texture(tex1, texCoord);
	vec3 ambient = ambientLight.rgb * ambientLight.a;
	fragColor = objColor * vec4(ambient, 1.0);
}
