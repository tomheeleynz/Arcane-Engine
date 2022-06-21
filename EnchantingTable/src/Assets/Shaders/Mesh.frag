#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec3 fragCameraPos;

layout(location = 0) out vec4 Color;

void main() {
	// Single Light Color
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	
	// Ambient Lighting
	float ambientStength = 0.1f;
	vec3 ambient = ambientStength * lightColor;

	// Diffuse Lighting
	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular Lighting
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(fragCameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	// Calculate Result
	vec3 result = (ambient + diffuse + specular) * vec3(1.0f, 0.0f, 0.0f);
	
	// Final Color
	Color = vec4(result, 1.0f);
}