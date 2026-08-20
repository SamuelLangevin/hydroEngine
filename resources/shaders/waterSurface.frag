#version 430 core

in float height;

out vec4 FragColor;

void main() {
    float h = (height + 16) /64.0f;
    FragColor = vec4(0.0, h, h, 1.0);
}