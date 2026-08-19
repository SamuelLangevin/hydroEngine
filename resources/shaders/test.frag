#version 430 core

in VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} fs_in;

out vec3 fragColor;

void main() {

    fragColor = vec3(fs_in.normal);
}