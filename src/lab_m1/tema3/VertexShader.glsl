#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 fragTexCoord;

void main()
{
    gl_Position = Projection * View * Model * vec4(position, 1.0);
    fragTexCoord = texCoord;
}
