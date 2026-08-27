#define PI 3.14159
#define e 2.71828

/** Stores the properties of a direcitonal wave */
struct DirectionalWave {
    vec2 direction;
    float waveLength;
    float amplitude;
    float speed;
};

/** Stores the properties of a point wave */
struct PointWave {
    vec2 origin;
    float waveLength;
    float amplitude;
    float speed;
    float dropTime;
};

/**
 * @param pWave the point wave to compute
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the height of the vertex
 */
float computePointWaveHeight(PointWave pWave, float absoluteTime, vec2 vertexPos){
    vec2 originToVert = pWave.origin - vertexPos;
    float relativeTime = absoluteTime - pWave.dropTime;
    float advance = pWave.speed * relativeTime;

    //todo replace magic 5, there could be better measures of speed and attenuation based on studies...
    float attenuation = pow(e, -advance/(pWave.amplitude * 5.0));
    float reachedPos = ceil(clamp(PI *advance - length(originToVert), 0.0, 1.0));
    float pointWaveHeight = pWave.amplitude * sin( -length(originToVert)/ pWave.waveLength + advance ) * attenuation * reachedPos;
    return pointWaveHeight;
}

/**
 * @param dWave the directional wave to compute
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the height of the vertex
 */
vec3 computeDirectionalWaveHeight(DirectionalWave dWave, float absoluteTime, vec3 vertexPos){
    vec3 newPos;
    float phase = dot(dWave.direction, vertexPos.xz)/dWave.waveLength - absoluteTime/sqrt(dWave.waveLength);
    newPos.x = vertexPos.x - dWave.amplitude * sin(phase);
    newPos.y = dWave.amplitude * cos(phase);
    newPos.z = vertexPos.z - dWave.amplitude * sin(phase);
    return newPos;
}