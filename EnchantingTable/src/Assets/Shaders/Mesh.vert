#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 0) out vec3 fragNormal;
layout (location = 1) out vec3 fragPos;
layout (location = 2) out vec3 fragCameraPos;
 
// Global Frame data
layout (set = 0, binding = 0) uniform Camera {
	mat4 proj;
	mat4 view;
	vec3 cameraPosition;
} camera;

// Per object data
layout (set = 3, binding = 0) uniform Model {
	mat4 transform;
} model;

void main() {
	gl_Position = camera.proj * camera.view * model.transform * vec4(aPos, 1.0);
	fragNormal = aNormal;
	fragPos = vec3(model.transform * vec4(aPos, 1.0));
}

