#version 450

layout (location = 0) in vec3 inPos;

layout (set = 0, binding = 0) uniform Camera {
	mat4 proj;
	mat4 view;
	vec3 cameraPosition;
} camera;

layout (location = 0) out vec3 fragTexCoords;

void main() {
	gl_Position = camera.proj * camera.view * vec4(inPos.xyz, 1.0);
	fragTexCoords = inPos;
	fragTexCoords.xy *= -1.0;
}