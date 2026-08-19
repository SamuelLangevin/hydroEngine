#version 430 core

struct DirectionalWave {
    vec2 direction;
    float waveLength;
    float magnitude;
    float speed;
};

in vec2 texCoords;

uniform float time;
uniform DirectionalWave dWave;

out vec4 fragColor;


void main() {
    float waveHeight = dWave.magnitude * sin((dot(dWave.direction, texCoords))/dWave.waveLength + time * dWave.speed) + 0.5;

    fragColor = vec4(0.0, 0.0, waveHeight, 1.0);
}