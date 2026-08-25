#version 330 core
#include "pbr/pbrFunctions.glsl"

in TESE_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;
uniform Material material;
uniform Environment environment;

out vec4 FragColor;

void main(){

    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(viewPos - fs_in.FragPos);
    vec3 albedo = vec3(texture(material.texture_diffuse0, fs_in.TexCoords));
    vec3 F_0 = mix(vec3(0.04), albedo, material.metallic);

    vec3 ambient = calcAmbientLight(environment, material, albedo, N, V, F_0);

    vec3 color = ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}