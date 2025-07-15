#version 320 es

layout (location = 0) in vec4 position;

uniform vec3 minCorner;
uniform vec3 maxCorner;
uniform mat4 worldMat;
uniform mat4 vpMat;

void main() {
	float x = mix(minCorner.x, maxCorner.x, position.x);
	float y = mix(minCorner.y, maxCorner.y, position.y);
	float z = mix(minCorner.z, maxCorner.z, position.z);
	gl_Position = vpMat * worldMat * vec4(x, y, z, 1.0);
}
