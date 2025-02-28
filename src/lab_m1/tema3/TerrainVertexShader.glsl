#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec2 texCoord;  
out float height;

uniform sampler2D heightmap;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform bool isSun;

void main() {
    
    vec3 updatedPosition = inPosition;

    if (!isSun) {
        float heightValue = texture(heightmap, inTexCoord).r;
        updatedPosition.y += heightValue * 50.0;
        height = heightValue;
    }
    
    texCoord = inTexCoord;
    gl_Position = Projection * View * Model * vec4(updatedPosition, 1.0);
}
