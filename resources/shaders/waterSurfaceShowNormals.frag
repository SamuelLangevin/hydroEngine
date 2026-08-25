#version 430 core

in TESE_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

void main() {
    FragColor = vec4(fs_in.Normal, 1.0);
}