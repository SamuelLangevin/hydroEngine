#define PI 3.14159
#define e 2.71828

struct DirectionalWave {
    vec2 direction;
    float waveLength;
    float magnitude;
    float speed;
};

struct PointWave {
    vec2 origin;
    float waveLength;
    float magnitude;
    float speed;
    float dropTime;
};

float computePointWaveHeight(PointWave pWave, float absoluteTime, vec2 vertexPos){
    vec2 originToVert = pWave.origin - vertexPos;
    float relativeTime = absoluteTime - pWave.dropTime;
    float advance = pWave.speed * relativeTime;

    //todo replace magic 10, there could be better measures of speed and attenuation based on studies...
    float attenuation = pow(e, -advance/(pWave.magnitude * 10.0));
    float reachedVertex = ceil(clamp(advance - length(originToVert), 0.0, 1.0));
    float pointWaveHeight = pWave.magnitude * sin( -length(originToVert)/ pWave.waveLength + advance ) * attenuation * reachedVertex;
    return pointWaveHeight;
}

float computeDirectionalWaveHeight(DirectionalWave dWave, float absoluteTime, vec2 vertexPos){
    return dWave.magnitude * sin(-dot(dWave.direction, vertexPos)/dWave.waveLength + absoluteTime * dWave.speed) + 0.5;
}