#version 450

layout(location = 0) out vec4 Color;

struct DirectionalLight 
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


void main() {
	Color = vec4(1.0, 0.0, 0.0, 1.0);
}