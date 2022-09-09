#version 450

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec3 fragPos;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec3 fragCameraPos;

layout(location = 0) out vec4 Color;

// Directional Light Struct
struct DirectionalLight {
	vec3 direction;
	vec3 color;
};

layout(set = 1, binding = 0) uniform Lights {
	DirectionalLight dirLight;
} lights;

// Material Structure
layout (set = 2, binding = 0) uniform Material {
	vec3 color;
} material;

layout (set = 2, binding = 1) uniform sampler2D albedo;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	// Get Light Direction
	vec3 lightDir = normalize(-light.direction);

	// Calculate diffuse light
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 ambient = light.color * vec3(texture(albedo, fragTexCoord));
	vec3 diffuse = light.color * diff *  vec3(texture(albedo, fragTexCoord));

	return (ambient + diffuse);
}

void main() {
	vec3 lightCalc = vec3(0.0);
	
	// Calculate Directional Light
	lightCalc += CalcDirLight(lights.dirLight, fragNormal, fragCameraPos);

	// Final Color
	Color = vec4(lightCalc, 1.0);
}

