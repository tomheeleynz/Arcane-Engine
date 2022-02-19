#version 450

layout (binding = 0) uniform UniformBufferObject {
    vec3 color;
} ubo;

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(ubo.color, 1.0);
}