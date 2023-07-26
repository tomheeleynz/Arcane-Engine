#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 fragTexCoord;

layout (set = 1, binding = 0) uniform sampler2D sprite;

layout (set = 1, binding = 1) uniform AnimData {
	float currentFrameCountX;
	float currentFrameCountY;
	float totalFrameCountX;
	float totalFrameCountY;
} animData;

layout(location = 0) out vec4 Color;

void main() {
	float animTexCoordX = (fragTexCoord.x + animData.currentFrameCountX) * (1.0 / animData.totalFrameCountX);
	float animTexCoordY = (fragTexCoord.y + animData.currentFrameCountY) * (1.0 / animData.totalFrameCountY);

	vec2 animTexCoords = vec2(animTexCoordX, animTexCoordY);
	Color = texture(sprite, animTexCoords) * vec4(fragColor, 1.0);
}