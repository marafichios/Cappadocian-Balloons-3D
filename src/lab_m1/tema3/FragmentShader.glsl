#version 330 core

in vec2 fragTexCoord;

uniform sampler2D texture_1;

out vec4 fragColor;

layout(location = 0) out vec4 out_color;


void main()
{
    fragColor = texture2D(texture_1, fragTexCoord);
    out_color = fragColor;
}
