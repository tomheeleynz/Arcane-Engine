#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int  aTexId;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragTexCoord;
layout (location = 2) out flat int fragTexId;

// Global Frame data
layout (set = 0, binding = 0) uniform Camera {
	mat4 proj;
	mat4 view;
	vec3 cameraPosition;
} camera;

void main() {
	gl_Position = camera.proj * camera.view * vec4(aPos, 1.0);
	fragColor = aColor;
	fragTexCoord = aTexCoord;
	fragTexId = aTexId;
}