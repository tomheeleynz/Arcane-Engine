#version 450
layout (location = 0) in vec3 fragTexCoords;

layout (set = 2, binding = 0) uniform samplerCube samplerCubeMap;
layout (location = 0) out vec4 Color;

void main() {
	Color = texture(samplerCubeMap, fragTexCoords);
}