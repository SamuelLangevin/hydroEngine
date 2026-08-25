#version 330 core
#include "pbr/sampling_functions.glsl"

in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

out vec4 FragColor;

void main() {
    vec3 N = normalize(localPos);
    vec3 R = N;
    vec3 V = R;
    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3(0.0);
    for(uint i = 0u; i < SAMPLE_COUNT; ++i){
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = importanceSamplingGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V,H) * H - V);

        float NdotL = max(dot(N,L), 0.0);
        if(NdotL > 0.0) {
            prefilteredColor += texture(environmentMap, L).rgb * NdotL;
            totalWeight += NdotL;
        }
    }

    prefilteredColor = prefilteredColor / totalWeight;
    FragColor = vec4(prefilteredColor, 1.0);
}