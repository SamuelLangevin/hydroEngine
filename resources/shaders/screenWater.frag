#version 430 core

struct DirectionalWave {
    vec2 direction;
    float waveLength;
    float magnitude;
    float speed;
};

struct PointWave {
    vec2 position;
    float waveLength;
    float magnitude;
    float speed;
};

in vec2 texCoords;

uniform float time;
uniform DirectionalWave dWave;
uniform PointWave pWave;

out vec4 fragColor;


void main() {

    float directWaveHeight = dWave.magnitude * sin(-dot(dWave.direction, texCoords)/dWave.waveLength + time * dWave.speed) + 0.5;
    vec2 posToFrag = pWave.position - texCoords;
    float pointWaveHeight = pWave.magnitude * sin(-dot(posToFrag, normalize(posToFrag))/pWave.waveLength + time * pWave.speed) + 0.5;

    fragColor = vec4(0.0, pointWaveHeight, directWaveHeight, 1.0);
}