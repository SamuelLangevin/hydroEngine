//https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation

#version 430 core
#include "waveFunctions.glsl"

layout (quads, fractional_odd_spacing, ccw) in;
in vec2 TextureCoord[];

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform float time;

#define MAX_NUMBER_POINT_WAVES 50
uniform PointWave pointWaves[MAX_NUMBER_POINT_WAVES];
uniform int nbOfPointWaves;

#define MAX_NUMBER_DIRECTIONAL_WAVES 50
uniform DirectionalWave directionalWaves[MAX_NUMBER_POINT_WAVES];
uniform int nbOfDirectionalWaves;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

//The position is assumed to be in world space
vec3 computeWavesDisplacement(vec4 position){
    vec3 newPosition = position.xyz;

    for (int i = 0; i < min(nbOfDirectionalWaves, MAX_NUMBER_DIRECTIONAL_WAVES); i++) {
        newPosition += computeDirectionalWave(directionalWaves[i], time, position.xyz);
    }

    for (int i = 0; i < min(nbOfPointWaves, MAX_NUMBER_POINT_WAVES); i++) {
        newPosition += computePointWave(pointWaves[i], time, position.xyz);
    }

    return newPosition;
}

//The position is assumed to be in world space
vec3 computeWavesNormal(vec4 position){
    vec3 binormal = vec3(1.0, 0.0, 0.0);
    vec3 tangent = vec3(0.0, 0.0, 1.0);

    for (int i = 0; i < min(nbOfDirectionalWaves, MAX_NUMBER_DIRECTIONAL_WAVES); i++) {
        binormal += computeDirectionalWaveBinormal(directionalWaves[i], time, position.xyz);
        tangent += computeDirectionalWaveTangent(directionalWaves[i], time, position.xyz);
    }

    for (int i = 0; i < min(nbOfPointWaves, MAX_NUMBER_POINT_WAVES); i++) {
        binormal += computePointWaveBinormal(pointWaves[i], time, position.xyz);
        tangent += computePointWaveTangent(pointWaves[i], time, position.xyz);

    }

    return normalize(cross(tangent, binormal));
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    TexCoords = (t1 - t0) * v + t0; //fixme texcoords will not be tied to a position

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize(vec4(cross(vVec.xyz, uVec.xyz), 0));

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 xzPos = (p1 - p0) * v + p0;

    vec4 p = vec4(computeWavesDisplacement(model * xzPos), 1.0);
    gl_Position = projection * view * p;
    FragPos = vec3(p);

    Normal = computeWavesNormal(model * xzPos);
}
