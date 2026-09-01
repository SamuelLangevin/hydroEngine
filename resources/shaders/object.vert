#version 430 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    FragPos = vec3(model * vec4(inPosition, 1.0));
    Normal = transpose(inverse(mat3(model))) * inNormal;
    TexCoords = inTexCoords;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}