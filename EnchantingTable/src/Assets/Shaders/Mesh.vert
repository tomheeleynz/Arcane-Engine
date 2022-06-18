#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Global Frame data
layout (set = 0, binding = 0) uniform Camera {
	mat4 proj;
	mat4 view;
} camera;

// Per object data
layout (set = 1, binding = 0) uniform Model {
	mat4 transform;
} model;

void main() {
	gl_Position = camera.proj * camera.view * model.transform * vec4(aPos, 1.0);
}

