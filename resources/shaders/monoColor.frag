#version 430 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

uniform vec4 color;

out vec4 fragColor;

void main() {

    fragColor = color;
}