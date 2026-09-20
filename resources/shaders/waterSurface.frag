#version 330 core
#include "pbr/pbrFunctions.glsl"

in vec3 FragPos;
in vec2 TexCoords;
in mat3 TBN;

uniform float time;
uniform vec3 viewPos;

uniform Material material;
uniform Environment environment;
uniform sampler2D oceanBedTexture;
uniform sampler2D noiseNormalMap;
uniform float depth;
uniform vec2 windVelocity;

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

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 refracted = refract(-V, N, 1.0/1.33);
    vec3 scatterColor = vec3(0.0, 0.5, 1.0);
    //vec3 scatterColor = textureLod(environment.prefilterMap, refracted, material.roughness * MAX_REFLECTION_LOD).rgb;


    float wrap = 0.5;
    float scatterWidth = 2.0;
    float NdotL = dot(N, vec3(1.0, -1.0, 1.0));
    float NdotL_wrap = (NdotL + wrap) / (1 + wrap);
    float diffuseC = max(NdotL_wrap, 0.0);
    float scatter = smoothstep(0.0, scatterWidth, NdotL_wrap) *
    smoothstep(scatterWidth * 2.0, scatterWidth, NdotL_wrap);

    vec3 values = scatter * scatterColor;

    vec3 deepColor = vec3(0.0, 0.0, 0.1);
    vec3 diffuse =  max(dot(N,V), 0.0) * values * texture(environment.irradianceMap, N).xyz + vec3(0.0, 0.0, 0.03);

    vec3 reflected = reflect(-V, N);
    vec3 prefilteredColor = textureLod(environment.prefilterMap, reflected, material.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(environment.brdfLUT, vec2(max(dot(N, V), 0.0), material.roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    return (kD * diffuse + specular) * material.ao;
}

void main(){
    //todo sub-surface scattering
    vec2 waterMovt = windVelocity * time;

    vec3 sample1 = normalize(texture(noiseNormalMap, TexCoords + waterMovt).rgb* 2.0 - 1.0);
    vec3 sample2 = normalize(texture(noiseNormalMap, TexCoords + waterMovt + vec2(0.1)).rgb* 2.0 - 1.0);
    vec3 normal = mix(sample1, sample2, 0.5*sin(4*time) + 0.5);
    normal = mix(vec3(0.0, 0.0, 1.0), normal, length(windVelocity)); //mixing two normal samples to not have static ripples

    vec3 N = TBN * normalize(normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 F_0 = mix(vec3(0.04), vec3(1.0), material.metallic);

    vec3 ambient = waterColor(N, V, F_0);

    vec3 color = ambient;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color, 1.0);
}