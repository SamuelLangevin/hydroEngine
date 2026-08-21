//https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation

#version 430 core
#include "waveFunctions.glsl"

layout (quads, fractional_odd_spacing, ccw) in;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform float time;
uniform DirectionalWave dWave;

#define MAX_NUMBER_POINT_WAVES 50
uniform PointWave pWaves[MAX_NUMBER_POINT_WAVES];
uniform int nbOfPointWaves;

out float height;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize(vec4(cross(vVec.xyz, uVec.xyz), 0));

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    vec4 modelSpacePoint = model * p;
    float directWaveHeight = computeDirectionalWaveHeight(dWave, time, modelSpacePoint.xz);

    float pointWaveHeightResult = 0.0;
    for (int i = 0; i < min(nbOfPointWaves, MAX_NUMBER_POINT_WAVES); i++) {
        pointWaveHeightResult += computePointWaveHeight(pWaves[i], time, modelSpacePoint.xz);
    }

    height = (directWaveHeight + pointWaveHeightResult) * 5.0;

    p += normal * height;

    gl_Position = projection * view * model * p;
}