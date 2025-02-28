#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec2 fragTexCoord;

void main()
{
	vec3 frag_position = position;
	fragTexCoord = texCoord;

	if (position.y < 0.0) {
		float scale = 1.0 + (position.y / -0.5);
		frag_position.y *= scale;
	}

	gl_Position = Projection * View * Model * vec4(frag_position, 1.0);
}