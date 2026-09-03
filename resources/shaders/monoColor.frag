#version 430 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec4 color;

out vec4 fragColor;

void main() {

    fragColor = color;
}