#version 450

layout (location = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; 

layout(location = 0) out vec3 fragColor;

void main() {
	gl_Position = vec4(aPos, 1.0);
	fragColor = aColor;
}