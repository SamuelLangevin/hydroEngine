#version 330 core
#include "pbr/pbrFunctions.glsl"

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Environment environment;

out vec4 FragColor;

void main(){

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 albedo = vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 F_0 = mix(vec3(0.04), albedo, material.metallic);

    vec3 ambient = calcAmbientLight(environment, material, albedo, N, V, F_0);

    vec3 color = ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}