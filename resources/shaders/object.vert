#version 430 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out VS_OUT {
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
} vs_out;

void main() {
    vs_out.fragPos = vec3(model * vec4(inPosition, 1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * inNormal;
    vs_out.texCoords = inTexCoords;
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
}