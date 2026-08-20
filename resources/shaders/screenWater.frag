#version 430 core
#include "waveFunctions.glsl"

in vec2 texCoords;

uniform float time;
uniform DirectionalWave dWave;

#define MAX_NUMBER_POINT_WAVES 50
uniform PointWave pWaves[MAX_NUMBER_POINT_WAVES];
uniform int nbOfPointWaves;

out vec4 fragColor;

void main() {

    float directWaveHeight = computeDirectionalWaveHeight(dWave, time, texCoords);

    float pointWaveHeightResult = 0.0;
    for (int i = 0; i < min(nbOfPointWaves, MAX_NUMBER_POINT_WAVES); i++) {
       pointWaveHeightResult += computePointWaveHeight(pWaves[i], time, texCoords);
    }

    fragColor = vec4(0.0, 0.0, directWaveHeight + pointWaveHeightResult, 1.0);
}