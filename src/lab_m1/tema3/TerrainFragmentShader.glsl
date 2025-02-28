#version 330 core

in vec2 texCoord;
in float height;

uniform sampler2D soilTexture;
uniform sampler2D grassTexture;
uniform bool isSun;

out vec4 FragColor;

void main() {

    if (isSun) {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    } else {
        vec4 terrainColor = texture(soilTexture, texCoord * 5.0);
        vec4 mountainColor = texture(grassTexture, texCoord * 5.0);
        float transitionStart = 0.1;
        float transitionEnd = 0.4;
        float mixFactor = smoothstep(transitionStart, transitionEnd, height); 
        FragColor = mix(mountainColor, terrainColor, mixFactor);
        //FragColor = terrainColor;
    }
   
}