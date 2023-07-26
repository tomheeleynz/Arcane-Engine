#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 fragTexCoord;
layout (location = 2) in flat int fragTexId; 

layout(location = 0) out vec4 Color;
layout(location = 1) out int EntityIds;

layout(set = 1, binding = 0) uniform sampler2D sprites[32];

void main() {
	EntityIds = 1;
	Color = texture(sprites[fragTexId], fragTexCoord) * vec4(fragColor, 1.0);
}
