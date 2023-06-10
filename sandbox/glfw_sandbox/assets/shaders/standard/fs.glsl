#version 320 es
precision mediump float;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D tex1;
uniform vec4 ambientLight;
uniform vec4 diffusePos;
uniform vec4 diffuseColor;
uniform vec3 viewPos;

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 fragPos;

out vec4 fragColor;

void main() {
	vec4 objColor = texture(tex1, texCoord);
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseIntensity = max(0.0, dot(surfaceNormal, lightDir));
	vec3 diffuse = light.diffuse * material.diffuse * diffuseIntensity;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;

	fragColor = vec4(ambient + diffuse + specular, 1.0);
}
