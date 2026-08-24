//Based on https://learnopengl.com/PBR/IBL/Specular-IBL

#version 330 core
#include "pbr/sampling_functions.glsl"
in vec2 TexCoords;

out vec2 FragColor;

float geometrySchlickGGX(float cosTheta, float roughness){
    float r = roughness;
    float k = (r*r) / 2.0;
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N,V), 0.0);
    float NdotL = max(dot(N,L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec2 integrateBRDF(float NdotV, float roughness){
    vec3 V;
    V.x = sqrt(1.0 - NdotV * NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    vec3 N = vec3(0.0, 0.0, 1.0);

    const uint SAMPLE_COUNT = 1024u;
    for (uint i = 0u; i < SAMPLE_COUNT; ++i){
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = importanceSamplingGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0){
            float G = geometrySmith(N, V, L, roughness);
            float G_vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_vis;
            B += Fc * G_vis;
        }
    }

    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);

   return vec2(A, B);
}

void main(){
    vec2 integral = integrateBRDF(TexCoords.x, TexCoords.y);
    FragColor = integral;
}