#version 330 core
#include "pbr/pbrFunctions.glsl"

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Environment environment;
uniform sampler2D oceanBedTexture;
uniform float depth;

out vec4 FragColor;

/**
 * @param albedo the diffuse color of the surface.
 * @param N normal of the fragment
 * @param V incoming direction of the camera's view
 * @param base reflectivity factor
 * @returns the environment's contribution to the surface's color
 */
vec3 waterColor(vec3 N, vec3 V, vec3 F_0) {
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F_0, material.roughness);
    vec3 kS = F; //Ratio of the light reflected
    vec3 kD = (1.0 - kS) * (1.0 - material.metallic); //Ratio of the light absorbed

    vec3 refracted = normalize(refract(-V, N, 1.0/1.33));
    vec3 oceanBedColor = texture(oceanBedTexture, refracted.xz).rgb;
    vec3 deepColor = vec3(0.0, 0.0, 0.1);
    vec3 shallowColor = vec3(0.0, 0.3, 0.3);
    vec3 depthColor = mix(deepColor, shallowColor + 0.2*oceanBedColor, depth);
    vec3 diffuse = max( dot(V, N),0 ) * depthColor * texture(environment.irradianceMap, N).xyz;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 reflected = reflect(-V, N);
    vec3 prefilteredColor = textureLod(environment.prefilterMap, reflected, material.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(environment.brdfLUT, vec2(max(dot(N, V), 0.0), material.roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    return (kD * diffuse + specular) * material.ao;
}

void main(){

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 F_0 = mix(vec3(0.04), vec3(1.0), material.metallic);

    vec3 ambient = waterColor(N, V, F_0);

    vec3 color = ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}