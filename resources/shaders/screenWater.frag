#version 430 core

#define PI 3.14159
#define e 2.71828

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
    float dropTime;
};

in vec2 texCoords;

uniform float time;
uniform DirectionalWave dWave;
uniform PointWave pWave;

out vec4 fragColor;


void main() {

    float directWaveHeight = dWave.magnitude * sin(-dot(dWave.direction, texCoords)/dWave.waveLength + time * dWave.speed) + 0.5;

    vec2 posToFrag = pWave.position - texCoords;
    float relativeTime = time - pWave.dropTime;
    float attenuation = pow(e, -pWave.speed * relativeTime/(pWave.magnitude * 20.0));
    //todo replace magic 20, there could be better measures of speed and attenuation based on studies...
    float reached = ceil(clamp(relativeTime/20.0 * pWave.speed - length(posToFrag), 0.0, 1.0));
    float pointWaveHeight = pWave.magnitude * sin( -dot(posToFrag, normalize(posToFrag))/pWave.waveLength + relativeTime * pWave.speed) * attenuation * reached;

    fragColor = vec4(0.0, pointWaveHeight, directWaveHeight, 1.0);
}