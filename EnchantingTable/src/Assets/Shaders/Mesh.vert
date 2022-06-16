#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Global Frame data
layout (set = 0, binding = 0) uniform MVP {
	mat4 proj;
	mat4 view;
} mvp;


void main() {
	gl_Position = vec4(aPos, 1.0);
}

