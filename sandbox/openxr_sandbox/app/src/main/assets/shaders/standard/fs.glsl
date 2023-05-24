#version 320 es
precision mediump float;

uniform sampler2D tex1;
uniform vec4 ambientLight;
uniform vec4 diffusePos;
uniform vec4 diffuseColor;
uniform vec4 viewPos;

in vec2 texCoord;
in vec3 surfaceNormal;
in vec3 fragPos;

out vec4 fragColor;

void main() {
	vec4 objColor = texture(tex1, texCoord);
	vec4 ambient = vec4(ambientLight.rgb * ambientLight.a, 1.0);

	vec3 lightDir = normalize(diffusePos.xyz - fragPos);
	float diffuseIntensity = max(0.0, diffuseColor.w * dot(surfaceNormal, lightDir));
	vec4 diffuse = diffuseColor * diffuseIntensity;

	float specularIntensity = 0.5;
	vec3 viewDir = normalize(viewPos.xyz - fragPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec4 specular = specularIntensity * spec * diffuseColor;

	fragColor = objColor * (ambient + diffuse + specular);
}
