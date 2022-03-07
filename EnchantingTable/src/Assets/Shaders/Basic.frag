#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout (binding = 0) uniform ColorObject {
    vec3 color;
} colorObject;

layout (binding = 1) uniform sampler2D texSampler;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = vec4(colorObject.color, 1.0);
}