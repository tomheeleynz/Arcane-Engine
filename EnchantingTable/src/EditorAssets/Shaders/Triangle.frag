#version 450
layout (location = 0) in vec2 fragTexCoord;

layout (location = 0) out vec4 FragColor;
void main()
{
    FragColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.0f, 1.0f);
} 